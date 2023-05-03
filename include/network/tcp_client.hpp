#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <network/connection.hpp>
#include <network/message.hpp>
#include <string>

using boost::asio::ip::tcp;

class TCPClient {
 public:
  TCPClient(boost::asio::io_context& io_context, std::string host,
            std::string port);
  void read();
  void write(message::Message);

 private:
  std::unique_ptr<Connection<message::Message>> connection;
  tcp::socket socket_;
  char data[1024];
};
