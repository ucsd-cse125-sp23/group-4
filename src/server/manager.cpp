#include <boost/asio.hpp>
#include <config/lib.hpp>
#include <memory>
#include <network/message.hpp>
#include <server/manager.hpp>
#include <unordered_map>

std::size_t Manager::MAX_PLAYERS{0};
std::chrono::seconds Manager::TOTAL_GAME_DURATION{0};

Manager::Manager()
    : io_context_(boost::asio::io_context()), timer_(io_context_) {
  auto config = get_config();
  MAX_PLAYERS = config["player_count"];
  TOTAL_GAME_DURATION = std::chrono::seconds{config["game_duration"]};

  game_ = std::make_unique<Game>();
}

int Manager::add_player() {
  int pid = game_->add_player();
  std::string default_skin = "trash panda";
  players_.insert({pid, {pid, default_skin, false, false}});

  return pid;
}

void Manager::remove_player(int pid) {
  game_->remove_player(pid);
  players_.erase(pid);
}

message::LobbyUpdate Manager::handle_lobby_update(
    const message::LobbyPlayerUpdate& update) {
  auto& player = players_.at(update.id);
  player.skin = update.skin;
  player.is_ready = update.is_ready;

  return get_lobby_update();
}

message::LobbyUpdate Manager::get_lobby_update() {
  std::unordered_map<int, message::LobbyPlayer> players;
  for (auto& [id, player] : players_)
    players.insert({id, {player.pid, player.skin, player.is_ready}});

  return {players};
}

bool Manager::check_ready() {
  int ready_count = 0;
  for (auto& [_, player] : players_)
    if (player.is_ready) ready_count++;

  bool is_ready = ready_count == MAX_PLAYERS;
  if (is_ready) status_ = Status::GameLoading;

  return is_ready;
}

void Manager::handle_game_loaded(const message::GameLoaded& body) {
  players_.at(body.pid).is_loaded = true;
}

bool Manager::check_loaded() {
  int loaded_count = 0;
  for (auto& [_, player] : players_)
    if (player.is_loaded) loaded_count++;

  bool is_loaded = loaded_count == MAX_PLAYERS;
  if (is_loaded) status_ = Status::InGame;

  return is_loaded;
}

void Manager::handle_game_update(const message::UserStateUpdate& update) {
  game_->update(update);
}

void Manager::tick_game() { game_->tick(); }

void Manager::start_game() {
  status_ = Status::InGame;
  game_->restart();

  // start game timer
  timer_.expires_from_now(TOTAL_GAME_DURATION);
  timer_.async_wait([this](const boost::system::error_code& _) {
    status_ = Status::GameOver;
  });
}

void Manager::poll() { io_context_.poll(); }

message::GameStateUpdate Manager::get_game_update() {
  return {game_->to_network()};
}
