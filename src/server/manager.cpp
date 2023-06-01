#include <network/message.hpp>
#include <server/manager.hpp>
#include <unordered_map>

int Manager::add_player() {
  int pid = game.add_player();
  players_.insert({pid, {pid, "", false}});

  return pid;
}

void Manager::remove_player(int pid) {
  game.remove_player(pid);
  players_.erase(pid);
}

message::LobbyUpdate Manager::handle_lobby_update(
    const message::LobbyPlayerUpdate& update) {
  if (update.is_ready) players_.at(update.id).is_ready = true;

  return get_lobby_update();
}

message::LobbyUpdate Manager::get_lobby_update() {
  std::unordered_map<int, message::LobbyPlayer> players;
  for (auto& [id, player] : players_)
    players.insert({id, {player.pid, player.skin, player.is_ready}});

  return {players};
}

bool Manager::check_ready() {
  int ready_count;
  for (auto& [_, player] : players_)
    if (player.is_ready) ready_count++;

  bool is_ready = ready_count == MAX_PLAYERS;
  if (is_ready) status_ = Status::InGame;

  return is_ready;
}

void Manager::handle_game_update(const message::UserStateUpdate& update) {
  game.update(update);
}

void Manager::tick_game() { game.tick(); }

message::GameStateUpdate Manager::get_game_update() {
  return {game.to_network()};
}
