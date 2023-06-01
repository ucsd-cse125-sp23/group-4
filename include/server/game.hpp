#pragma once

#include <core/lib.hpp>
#include <network/message.hpp>
#include <unordered_map>
#include <unordered_set>

#include "client/graphics/ColliderImporter.h"

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

  int id;
  float heading;
  Player* player;
  ControlModifierData* control;
};

// stored in server
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
};

class Manager {
 public:
  enum class Status {
    Lobby,
    InGame,
    GameOver,
  };

  static const std::size_t MAX_PLAYERS = 2;

  message::LobbyUpdate handle_lobby_update(message::LobbyPlayerUpdate&);
  bool check_ready();
  void handle_game_update(message::UserStateUpdate&);
  message::GameStateUpdate get_game_update();

  Status status_ = Status::Lobby;

 private:
  std::unordered_set<int> ready_players;
};
