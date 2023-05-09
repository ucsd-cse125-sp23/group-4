#pragma once

#include <boost/asio.hpp>
#include <cstddef>
#include <functional>
#include <memory>
#include <network/connection.hpp>
#include <network/message.hpp>
#include <string>

using boost::asio::ip::tcp;

struct Addr {
  std::string host;
  int port;
};

class Client {
 public:
  using ConnectHandler = std::function<void(tcp::endpoint, Client &)>;
  using ReadHandler = std::function<void(const message::Message &, Client &)>;
  using WriteHandler = std::function<void(std::size_t, Client &)>;
  Client(boost::asio::io_context &, Addr &, ConnectHandler, ReadHandler,
         WriteHandler);
  void read();
  void write(message::Message);

 private:
  std::unique_ptr<Connection<message::Message>> connection;
  tcp::socket socket_;
};
