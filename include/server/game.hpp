#pragma once

#include <core/lib.hpp>
#include <network/message.hpp>
#include <unordered_map>
#include <vector>

#include "client/graphics/MapDataImporter.h"
#include "core/game/event/Event.h"

// represents a new Player right now
// TODO: figure out how to support items later on
class GameThing {
 public:
  GameThing(int, Player*, ControlModifierData*, Level*);

  void update(const message::UserStateUpdate& update);
  void remove();
  bool is_tagged() const;
  message::Player to_network() const;

 private:
  Level* level_;
  int id_;
  float heading_;
  Player* player_;
  ControlModifierData* control_;
};

class Game {
 public:
  Game();

  int add_player();
  void remove_player(int);
  void update(const message::UserStateUpdate&);
  void tick();
  void restart();
  std::vector<message::JumpEvent> get_jump_events();
  std::vector<message::LandEvent> get_land_events();
  std::vector<message::ItemPickupEvent> get_item_pickup_events();
  std::vector<message::TagEvent> get_tag_events();
  void clear_events();
  message::GameStateUpdate to_network();

 private:
  std::unordered_map<int, GameThing> game_things_;
  std::vector<JumpEvent> jump_events_;
  std::vector<LandEvent> land_events_;
  std::vector<PickupEvent> item_pickup_events_;
  std::vector<TaggingEvent> tag_events_;
  int tagged_player_ = -1;
  Level* level_;
  std::vector<vec3f> map_spawn_points;
  // TODO: add other map things here...
};
