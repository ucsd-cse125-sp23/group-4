#pragma once

#include <boost/asio.hpp>
#include <chrono>
#include <network/message.hpp>
#include <server/game.hpp>
#include <string>
#include <unordered_map>

class Manager {
 public:
  enum class Status {
    Lobby,
    InGame,
    GameOver,
  };

  static std::size_t MAX_PLAYERS;
  static std::chrono::seconds TOTAL_GAME_DURATION;

  Manager();

  int add_player();
  void remove_player(int);
  message::LobbyUpdate handle_lobby_update(const message::LobbyPlayerUpdate&);
  message::LobbyUpdate get_lobby_update();
  bool check_ready();
  void handle_game_update(const message::UserStateUpdate&);
  void tick_game();
  void start_game();
  message::GameStateUpdate get_game_update();

  Status status_ = Status::Lobby;
  Game game_;

 private:
  struct Player {
    int pid;
    std::string skin;
    bool is_ready;
  };

  boost::asio::io_context io_context_;
  boost::asio::steady_timer timer_;
  std::unordered_map<int, Player> players_;
};
