#pragma once

#include <core/lib.hpp>
#include <network/message.hpp>
#include <unordered_map>

#include "client/graphics/ColliderImporter.h"

struct GameThing {
  int id;
  float heading;
  Player* player;
  ControlModifierData* control;

  GameThing(int&, Player*, ControlModifierData*);
  void move(float, float, float);  // NOLINT
  void update(const message::UserStateUpdate& update);
  message::GameStateUpdateItem to_network();
};

using GameThingMap = std::unordered_map<int, GameThing>;

// stored in server
class Game {
 public:
  Game();

  int create_player();
  void update(const message::UserStateUpdate&);
  void tick();
  std::unordered_map<int, message::GameStateUpdateItem> to_network();

 private:
  GameThingMap game_things_;
  int next_pid = 1;
};
