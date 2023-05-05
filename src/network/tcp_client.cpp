#include <iostream>
#include <memory>
#include <network/message.hpp>
#include <network/tcp_client.hpp>

TCPClient::TCPClient(boost::asio::io_context& io_context, Addr& addr,
                     ConnectHandler ch, ReadHandler rh, WriteHandler wh)
    : socket_(io_context) {
  tcp::resolver resolver(io_context);
  boost::asio::async_connect(
      socket_, resolver.resolve(addr.host, addr.port),
      [&](boost::system::error_code ec, tcp::endpoint endpoint) {
        if (ec) {
          std::cerr << "Error: " << ec.message() << std::endl;
          return;
        }

        auto read_handler = [&](boost::system::error_code ec,
                                const message::Message& m) {
          if (ec) {
            std::cerr << "Error: " << ec.message() << std::endl;
            return;
          }

          rh(m, *this);
          read();
        };
        auto write_handler = [&](boost::system::error_code ec,
                                 std::size_t bytes_transferred) {
          if (ec) {
            std::cerr << "Error: " << ec.message() << std::endl;
            return;
          }

          wh(bytes_transferred, *this);
        };

        connection = std::make_unique<Connection<message::Message>>(
            socket_, read_handler, write_handler);
        ch(endpoint, *this);
        read();
      });
}

void TCPClient::read() {
  std::cout << "Entering read state." << std::endl;
  connection->read();
}

void TCPClient::write(message::Message message) { connection->write(message); }
