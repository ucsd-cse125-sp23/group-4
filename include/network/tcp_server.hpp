#pragma once

#include <boost/asio.hpp>
#include <boost/container_hash/hash.hpp>
#include <chrono>
#include <memory>
#include <network/connection.hpp>
#include <network/message.hpp>
#include <unordered_map>
#include <vector>

using boost::asio::ip::tcp;
using message::PlayerID;

class Server {
 public:
  Server(boost::asio::io_context& io_context, int port);

 private:
  int update_num_ = 1;
  std::chrono::milliseconds tick_rate_ = std::chrono::milliseconds(2000);
  boost::asio::steady_timer timer_;
  tcp::acceptor acceptor_;
  std::unordered_map<PlayerID, std::unique_ptr<Connection<message::Message>>,
                     boost::hash<PlayerID>>
      connections_;

  friend std::ostream& operator<<(std::ostream&, Server*);
  void do_accept();
  void read(const PlayerID&);
  void write(const message::Message&, const PlayerID&);
  void write_all(message::Message&);
  void tick();
};
