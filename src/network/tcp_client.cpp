#include <boost/variant.hpp>
#include <cstddef>
#include <iostream>
#include <memory>
#include <network/message.hpp>
#include <network/tcp_client.hpp>

Client::Client(boost::asio::io_context& io_context, Addr& addr,
               ConnectHandler connect_handler, ReadHandler read_handler,
               WriteHandler write_handler)
    : socket_(io_context) {
  tcp::resolver resolver(io_context);
  auto endpoints = resolver.resolve(addr.host, std::to_string(addr.port));
  boost::asio::async_connect(
      socket_, endpoints,
      [=](boost::system::error_code ec, tcp::endpoint endpoint) {
        if (ec) {
          std::cerr << "Error: " << ec.message() << std::endl;
          return;
        }

        std::cout << "(Client::connect) Connected to " << endpoint.address()
                  << ":" << endpoint.port() << std::endl;

        auto conn_read_handler = [=](boost::system::error_code ec,
                                     const message::Message& m) {
          if (ec) return;

          // save player_id assigned by the server
          if (const message::Assign* body =
                  boost::get<message::Assign>(&m.body)) {
            id_ = m.metadata.id;
          }

          read_handler(m, *this);
          read();
        };

        auto conn_write_handler = [=](boost::system::error_code ec,
                                      std::size_t bytes_transferred,
                                      const message::Message& m) {
          if (ec) return;

          write_handler(bytes_transferred, m, *this);
        };

        connection = std::make_unique<Connection<message::Message>>(
            socket_, conn_read_handler, conn_write_handler);
        connect_handler(endpoint, *this);
        read();
      });
}

void Client::read() {
  // std::cout << "Queueing read handler" << std::endl;
  connection->read();
}

void Client::write(message::Message m) {
  // std::cout << "Queueing write to server: " << m << std::endl;
  connection->write(m);
}
