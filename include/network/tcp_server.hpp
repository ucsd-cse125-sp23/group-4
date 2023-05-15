#pragma once

#include <boost/asio.hpp>
#include <boost/container_hash/hash.hpp>
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
  friend std::ostream& operator<<(std::ostream&, Server*);

  tcp::acceptor acceptor_;
  std::unordered_map<PlayerID, std::unique_ptr<Connection<message::Message>>,
                     boost::hash<PlayerID>>
      connections_;

  void do_accept();
  void read(const PlayerID&);
  void write(const message::Message&, const PlayerID&);
  void write_all(const message::Message&);
};
