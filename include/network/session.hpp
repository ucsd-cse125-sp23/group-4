#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

class Session : std::enable_shared_from_this<Session> {
 public:
  explicit Session(tcp::socket socket);
  void start();

 private:
  tcp::socket socket_;
  char data_[1024];
  void read();
  void write(std::size_t length);
};
