#include <algorithm>
#include <core/lib.hpp>
#include <network/message.hpp>
#include <server/game.hpp>

GameThing::GameThing(int& id, Player* p, ControlModifierData* c)
    : id(id), player(p), control(c), heading(0) {}

void GameThing::move(float x, float y, float z) {  // NOLINT
  control->horizontalVel = vec3f(x, y, z);
}

void GameThing::update(const message::UserStateUpdate& update) {
  std::cout << "(GameThing::update) Updating GameThing " << update.id
            << std::endl;
  control->horizontalVel = vec3f(update.movx, update.movy, update.movz);
  control->doJump = update.jump;
  heading = update.heading;
}

message::GameStateUpdateItem GameThing::to_network() {
  return {id, player->getPos().x, player->getPos().y, player->getPos().z,
          heading};
}

Game::Game() {
  Environment* environment = new Environment();
  std::vector<ColliderData> mapColliders =
      ColliderImporter::ImportCollisionData("assets/models/test_colliders.obj");
  for (auto collider : mapColliders) {
    environment->addConvex(collider.vertices, 0.2f);
  }
  initializeLevel(environment);
}

int Game::create_player() {
  auto player_pair = initializePlayer();
  int new_id = next_pid++;
  game_things_.insert(
      {new_id, GameThing(new_id, player_pair.first, player_pair.second)});
  return new_id;
}

void Game::update(const message::UserStateUpdate& update) {
  // TODO: remove this when proper assignment is implemented
  game_things_.at(update.id).update(update);
}

void Game::tick() { level->tick(); }

std::unordered_map<int, message::GameStateUpdateItem> Game::to_network() {
  std::unordered_map<int, message::GameStateUpdateItem> things(
      game_things_.size());
  std::transform(game_things_.begin(), game_things_.end(),
                 std::inserter(things, things.end()), [](auto&& thing) {
                   return std::pair<int, message::GameStateUpdateItem>(
                       thing.first, thing.second.to_network());
                 });
  return things;
}
