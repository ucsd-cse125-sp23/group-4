#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <network/connection.hpp>
#include <network/message.hpp>
#include <string>

using boost::asio::ip::tcp;

struct Addr {
  std::string host;
  std::string port;
};

class TCPClient {
 public:
  TCPClient(boost::asio::io_context& io_context, Addr& addr);
  void read();
  void write(message::Message);

 private:
  std::unique_ptr<Connection<message::Message>> connection;
  tcp::socket socket_;
  char data[1024];
};
