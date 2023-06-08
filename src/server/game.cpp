#include <algorithm>
#include <config/lib.hpp>
#include <core/lib.hpp>
#include <network/item.hpp>
#include <network/message.hpp>
#include <server/game.hpp>

#include "core/game/event/Event.h"
#include "core/game/mode/GameModes.h"
#include "core/game/modifier/TaggedStatusModifier.h"
#include "core/util/global.h"

GameThing::GameThing(int id, Player* p, ControlModifierData* c, Level* l)
    : id_(id), player_(p), control_(c), heading_(0), level_(l) {}

void GameThing::move(float x, float y, float z) {  // NOLINT
  control_->horizontalVel = vec3f(x, y, z);
}

void GameThing::update(const message::UserStateUpdate& update) {
  control_->horizontalVel = vec3f(update.movx, update.movy, update.movz);
  control_->doJump = update.jump;
  heading_ = update.heading;
}

void GameThing::remove() { player_->markRemove(); }

bool GameThing::is_tagged() const {
  return TaggedStatusModifier::isIt(player_);
}

message::GameStateUpdateItem GameThing::to_network() const {
  return {
      id_,
      player_->getPos().x,
      player_->getPos().y,
      player_->getPos().z,
      heading_,
      level_->gameMode->queryScore(id_),
      length(player_->vel),
      player_->onGround,
      is_tagged(),
      {}  // TODO: get player effects
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
  applyGameMode(level_, new OneTaggerTimeGameMode());

  // register event handlers
  auto jump_handler = [this](JumpEvent&& e) { jump_events_.push_back(e); };
  auto land_handler = [this](LandEvent&& e) { land_events_.push_back(e); };
  auto item_pickup_handler = [this](PickupEvent&& e) {
    item_pickup_events_.push_back(e);
  };
  auto tag_handler = [this](TaggingEvent&& e) {
    tagged_player_ =
        static_cast<Player*>(e.tagee)->pid;  // changed tagged player
    tag_events_.push_back(e);
  };
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
  level_->restartGame();

  // assign first tagged player
  for (auto& [pid, thing] : game_things_)
    if (thing.is_tagged()) tagged_player_ = pid;
}

// note: PObject.id != Player.pid in event handlers below :((

std::vector<message::JumpEvent> Game::get_jump_events() {
  std::vector<message::JumpEvent> events;
  for (auto& e : jump_events_)
    events.push_back({static_cast<int>(static_cast<Player*>(e.self)->pid)});

  return events;
}

std::vector<message::LandEvent> Game::get_land_events() {
  std::vector<message::LandEvent> events;
  for (auto& e : land_events_)
    events.push_back({static_cast<int>(static_cast<Player*>(e.self)->pid)});

  return events;
}

std::vector<message::ItemPickupEvent> Game::get_item_pickup_events() {
  std::vector<message::ItemPickupEvent> events;
  for (auto& e : land_events_)
    events.push_back(
        {static_cast<int>(static_cast<Player*>(e.self)->pid), Item::GiftBox});

  return events;
}

std::vector<message::TagEvent> Game::get_tag_events() {
  std::vector<message::TagEvent> events;
  for (auto& e : tag_events_)
    events.push_back({static_cast<int>(static_cast<Player*>(e.tagger)->pid),
                      static_cast<int>(static_cast<Player*>(e.tagee)->pid)});

  return events;
}

void Game::clear_events() {
  jump_events_.clear();
  land_events_.clear();
  item_pickup_events_.clear();
  tag_events_.clear();
}

message::GameStateUpdate Game::to_network() {
  std::unordered_map<int, message::GameStateUpdateItem> things;
  for (const auto& [pid, thing] : game_things_)
    things.insert({pid, thing.to_network()});

  float time_elapsed = (level_->getAge() - TAG_COOLDOWN) / 20.0;
  return {things, tagged_player_, time_elapsed};
}
