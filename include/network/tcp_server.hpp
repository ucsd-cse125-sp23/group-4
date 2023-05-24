#pragma once

#include <boost/asio.hpp>
#include <boost/container_hash/hash.hpp>
#include <chrono>
#include <memory>
#include <network/connection.hpp>
#include <network/game.hpp>
#include <network/message.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

using boost::asio::ip::tcp;
using message::PlayerID;

class Server {
 public:
  Server(boost::asio::io_context& io_context, int port);
  constexpr static std::chrono::milliseconds TICK_RATE =
      std::chrono::milliseconds(50);

 private:
  void do_accept();
  void tick();
  void read(const ClientID&);
  void write(const ClientID&, const message::Message&);
  void write_all(message::Message&);
  template <typename T, typename... Args>
  void write(const ClientID&, Args&&...);
  template <typename T, typename... Args>
  void write_all(Args&&...);

  friend std::ostream& operator<<(std::ostream&, Server*);

  Game game_;
  boost::asio::steady_timer timer_;
  tcp::acceptor acceptor_;
  std::unordered_map<ClientID, std::unique_ptr<Connection<message::Message>>,
                     boost::hash<ClientID>>
      connections_;
};

template <typename T, typename... Args>
void Server::write(const ClientID& player_id, Args&&... args) {
  T body{std::forward<Args>(args)...};
  message::Type type = message::get_type(body);
  message::Metadata metadata{player_id, std::time(nullptr)};
  message::Message m{type, metadata, body};
  write(player_id, m);
}

template <typename T, typename... Args>
void Server::write_all(Args&&... args) {
  for (const auto& kv : connections_) {
    T body{std::forward<Args>(args)...};
    message::Type type = message::get_type(body);
    message::Metadata metadata{kv.first, std::time(nullptr)};
    message::Message m{type, metadata, body};
    write(kv.first, m);
  }
}
