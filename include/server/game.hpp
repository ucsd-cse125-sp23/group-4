#pragma once

#include <core/lib.hpp>
#include <network/message.hpp>
#include <unordered_map>

#include "client/graphics/MapDataImporter.h"

// represents a new Player right now
// TODO: figure out how to support items later on
class GameThing {
 public:
  GameThing(int, Player*, ControlModifierData*);

  void update(const message::UserStateUpdate& update);
  void remove();
  message::GameStateUpdateItem to_network() const;

 private:
  void move(float, float, float);  // NOLINT

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
  std::unordered_map<int, message::GameStateUpdateItem> to_network();

 private:
  std::unordered_map<int, GameThing> game_things_;

  std::vector<vec3f> map_spawn_points;
  // TODO: add other map things here...
};
