#include <iostream>
#include <memory>
#include <network/message.hpp>
#include <network/tcp_client.hpp>

TCPClient::TCPClient(boost::asio::io_context& io_context, Addr& addr,
                     ConnectHandler connect_handler, ReadHandler read_handler,
                     WriteHandler write_handler)
    : socket_(io_context) {
  tcp::resolver resolver(io_context);

  boost::asio::async_connect(
      socket_, resolver.resolve(addr.host, std::to_string(addr.port)),
      [&, read_handler, write_handler, connect_handler](
          boost::system::error_code ec, tcp::endpoint endpoint) {
        if (ec) {
          std::cerr << "Error: " << ec.message() << std::endl;
          return;
        }

        auto conn_read_handler = [&, read_handler](boost::system::error_code ec,
                                                   const message::Message& m) {
          if (ec) {
            std::cerr << "Error: " << ec.message() << std::endl;
            return;
          }

          read_handler(m, *this);
        };

        auto conn_write_handler = [&, write_handler](
                                      boost::system::error_code ec,
                                      std::size_t bytes_transferred) {
          if (ec) {
            std::cerr << "Error: " << ec.message() << std::endl;
            return;
          }

          write_handler(bytes_transferred, *this);
        };

        connection = std::make_unique<Connection<message::Message>>(
            socket_, conn_read_handler, conn_write_handler);
        connect_handler(endpoint, *this);
        read();
      });
}

void TCPClient::read() {
  std::cout << "Entering read state." << std::endl;
  connection->read();
}

void TCPClient::write(message::Message message) { connection->write(message); }
