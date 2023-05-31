#include <algorithm>
#include <core/lib.hpp>
#include <network/message.hpp>
#include <server/game.hpp>

GameThing::GameThing(int id, Player* p, ControlModifierData* c)
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

message::GameStateUpdateItem GameThing::to_network() const {
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
  auto [player, control] = initializePlayer();
  game_things_.insert({player->pid, GameThing(player->pid, player, control)});
  return player->pid;
}

void Game::remove_player(int pid) {
  game_things_.at(pid).player->markRemove();
  game_things_.erase(pid);
}

void Game::update(const message::UserStateUpdate& update) {
  game_things_.at(update.id).update(update);
}

void Game::tick() { level->tick(); }

std::unordered_map<int, message::GameStateUpdateItem> Game::to_network() {
  std::unordered_map<int, message::GameStateUpdateItem> things;
  for (const auto& [pid, thing] : game_things_)
    things.insert({pid, thing.to_network()});
  return things;
}
