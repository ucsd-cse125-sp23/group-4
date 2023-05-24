#pragma once

#include <boost/asio.hpp>
#include <cstddef>
#include <functional>
#include <memory>
#include <network/connection.hpp>
#include <network/message.hpp>
#include <string>
#include <utility>

using boost::asio::ip::tcp;

struct Addr {
  std::string host;
  int port;
  friend std::ostream &operator<<(std::ostream &os, const Addr &a) {
    return os << a.host << ":" << a.port;
  }
};

class Client {
 public:
  using ConnectHandler = std::function<void(tcp::endpoint, Client &)>;
  using ReadHandler = std::function<void(const message::Message &, Client &)>;
  using WriteHandler =
      std::function<void(std::size_t, const message::Message &, Client &)>;

  Client(boost::asio::io_context &, Addr &, ConnectHandler, ReadHandler,
         WriteHandler);

  void read();
  void write(message::Message);
  template <typename T, typename... Args>
  void write(Args &&...);

  message::ClientID id_;

 private:
  std::unique_ptr<Connection<message::Message>> connection;
  tcp::socket socket_;
};

template <typename T, typename... Args>
void Client::write(Args &&...args) {
  T body{std::forward<Args>(args)...};
  message::Type type = message::get_type(body);
  message::Metadata metadata{id_, std::time(nullptr)};
  message::Message m{type, metadata, body};
  connection->write(m);
}
