#pragma once

#include <boost/asio.hpp>
#include <network/connection.hpp>
#include <network/message.hpp>
#include <string>

using boost::asio::ip::tcp;

class TCPClient {
 public:
  TCPClient(boost::asio::io_context& io_context, std::string host,
            std::string port);
  message::Message read();
  void write(message::Message);

 private:
  Connection connection;
  tcp::socket socket_;
  char data[1024];
};
