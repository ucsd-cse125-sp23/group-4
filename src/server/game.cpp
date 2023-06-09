#include <algorithm>
#include <chrono>
#include <config/lib.hpp>
#include <core/lib.hpp>
#include <network/item.hpp>
#include <network/message.hpp>
#include <server/game.hpp>
#include <server/manager.hpp>
#include <unordered_map>

int get_pid(PObject* p) { return static_cast<Player*>(p)->pid; }

using core::Player;

GameThing::GameThing(int id, Player* p, ControlModifierData* c, Level* l)
    : id_(id), player_(p), control_(c), heading_(0), level_(l) {}

void GameThing::update(const message::UserStateUpdate& update) {
  control_->horizontalVel = vec3f(update.movx, update.movy, update.movz);
  control_->doJump = update.jump;
  heading_ = update.heading;
}

void GameThing::remove() { player_->markRemove(); }

bool GameThing::is_tagged() const {
  return TaggedStatusModifier::isIt(player_);
}

int GameThing::get_score() const {
  return level_->getAge() - TAG_COOLDOWN - level_->gameMode->queryScore(id_);
}

message::Player GameThing::to_network() const {
  auto effects = EffectStorageModifier::queryEffects(player_);

  return {
      id_,
      player_->getPos().x,
      player_->getPos().y,
      player_->getPos().z,
      heading_,
      get_score(),
      length(player_->vel),
      player_->ticksFallen,
      player_->onGround,
      is_tagged(),
      isMoving(player_),
      effects,
  };
}

Game::Game() {
  auto config = get_config();

  Environment* environment = new Environment();
  MapData mapData = MapDataImporter::Import(config["map_data_file"]);
  for (auto collider : mapData.colliders) {
    environment->addConvex(collider.vertices, 2.4f);
  }
  for (auto zerof : mapData.groups["zerof"]) {
    environment->addConvex(zerof.vertices, 0.0f);
  }
  for (auto spawn : mapData.spawnPoints) {
    map_spawn_points.push_back(spawn.point);
    environment->addPlayerSpawnpoint(spawn.point);
  }
  for (auto spawn : mapData.itemPoints) {
    environment->addItemSpawnpoint(spawn.point);
  }
  environment->setDeathHeight(mapData.fallBoundY);
  level_ = initializeLevel(environment);
  if (config["game_mode"] == "single-tagger")
    applyGameMode(level_, new NTaggersTimeGameMode(1));
  else if (config["game_mode"] == "multi-tagger")
    applyGameMode(level_, new NTaggersTimeGameMode(Manager::MAX_PLAYERS - 1));
  else
    applyGameMode(level_, new NTaggersTimeGameMode(1));

  // register event handlers
  auto jump_handler = [this](JumpEvent&& e) { jump_events_.push_back(e); };
  auto land_handler = [this](LandEvent&& e) { land_events_.push_back(e); };
  auto item_pickup_handler = [this](PickupEvent&& e) {
    item_pickup_events_.push_back(e);
  };
  auto tag_handler = [this](TaggingEvent&& e) { tag_events_.push_back(e); };
  level_->eventManager->registerEventHandler(jump_handler);
  level_->eventManager->registerEventHandler(land_handler);
  level_->eventManager->registerEventHandler(item_pickup_handler);
  level_->eventManager->registerEventHandler(tag_handler);
}

int Game::add_player() {
  auto [player, control] = initializePlayer(level_);
  game_things_.insert(
      {player->pid, GameThing(player->pid, player, control, level_)});
  if (map_spawn_points.size() > 0) player->setPos(map_spawn_points[0]);
  return player->pid;
}

void Game::remove_player(int pid) {
  game_things_.at(pid).remove();
  game_things_.erase(pid);
}

void Game::update(const message::UserStateUpdate& update) {
  game_things_.at(update.id).update(update);
}

void Game::tick() { level_->tick(); }

void Game::restart() {
  clear_events();
  level_->restartGame();
  start_time_ = std::chrono::steady_clock::now();
}

std::vector<message::JumpEvent> Game::get_jump_events() {
  std::vector<message::JumpEvent> events;
  for (auto& e : jump_events_) events.push_back({get_pid(e.self)});

  return events;
}

std::vector<message::LandEvent> Game::get_land_events() {
  std::vector<message::LandEvent> events;
  for (auto& e : land_events_) events.push_back({get_pid(e.self)});

  return events;
}

std::vector<message::ItemPickupEvent> Game::get_item_pickup_events() {
  std::vector<message::ItemPickupEvent> events;
  for (auto& e : item_pickup_events_)
    events.push_back({get_pid(e.self), Item::RedGiftBox});

  return events;
}

std::vector<message::TagEvent> Game::get_tag_events() {
  std::vector<message::TagEvent> events;
  for (auto& e : tag_events_)
    events.push_back({get_pid(e.tagger), get_pid(e.tagee)});

  return events;
}

std::unordered_map<int, int> Game::get_scores() {
  std::unordered_map<int, int> scores;
  for (auto& [pid, thing] : game_things_) scores[pid] = thing.get_score();

  return scores;
}

void Game::clear_events() {
  jump_events_.clear();
  land_events_.clear();
  item_pickup_events_.clear();
  tag_events_.clear();
}

message::GameStateUpdate Game::to_network() {
  std::unordered_map<int, message::Player> players;
  for (const auto& [pid, player] : game_things_)
    players.insert({pid, player.to_network()});

  std::unordered_map<int, message::Item> items;
  for (auto& powerup : getPowerUps(level_))
    items[powerup->id] = {static_cast<int>(powerup->id), powerup->item,
                          powerup->getPos().x, powerup->getPos().y,
                          powerup->getPos().z};

  auto now = std::chrono::steady_clock::now();
  auto time_elapsed = now - start_time_;
  auto time_remaining = Manager::TOTAL_GAME_DURATION - time_elapsed;

  auto time_elapsed_s =
      std::chrono::duration_cast<std::chrono::duration<float>>(time_elapsed)
          .count();

  auto time_remaining_s =
      std::chrono::duration_cast<std::chrono::duration<float>>(time_remaining)
          .count();

  return {players, items, time_elapsed_s, time_remaining_s};
}
