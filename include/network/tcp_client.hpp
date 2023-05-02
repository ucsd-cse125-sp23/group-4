#pragma once

#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;

class TCPClient {
 public:
  TCPClient(boost::asio::io_context& io_context, std::string host,
            std::string port);
  std::string read();
  void write(std::string);

 private:
  tcp::socket socket_;
  char data[1024];
};
