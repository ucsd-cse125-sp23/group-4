#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <network/connection.hpp>
#include <network/message.hpp>
#include <vector>

using boost::asio::ip::tcp;

class TCPServer {
 public:
  TCPServer(boost::asio::io_context& io_context, int port);

 private:
  tcp::acceptor acceptor_;
  std::vector<std::shared_ptr<Connection<message::Message>>> connections_;

  void do_accept();
  void read(int);
  void write(const message::Message&, int);
  void write_all(const message::Message&);
};
