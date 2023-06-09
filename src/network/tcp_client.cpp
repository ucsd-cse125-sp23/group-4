#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <network/message.hpp>
#include <network/tcp_client.hpp>

Client::Client(Addr& addr, ConnectHandler connect_handler,
               ReadHandler read_handler, WriteHandler write_handler)
    : io_context_(boost::asio::io_context()), socket_(io_context_) {
  tcp::resolver resolver(io_context_);
  auto endpoints = resolver.resolve(addr.host, std::to_string(addr.port));
  std::cout << "(Client::Client) Attempting to connect to " << addr
            << std::endl;
  boost::asio::async_connect(
      socket_, endpoints,
      [this, connect_handler, read_handler, write_handler](
          boost::system::error_code ec, tcp::endpoint endpoint) {
        if (ec) {
          std::cerr << "Error: " << ec.message() << std::endl;
          return;
        }

        std::cout << "(Client::async_connect) Connected to "
                  << endpoint.address() << ":" << endpoint.port() << std::endl;

        auto conn_read_handler = [this, read_handler](
                                     boost::system::error_code ec,
                                     const message::Message& m) {
          if (ec) return;

          // save client_id assigned by the server
          if (const message::Assign* body =
                  boost::get<message::Assign>(&m.body)) {
            id_ = m.metadata.id;
          }

          read_handler(m, *this);
        };

        auto conn_write_handler = [this, write_handler](
                                      boost::system::error_code ec,
                                      std::size_t bytes_transferred,
                                      const message::Message& m) {
          if (ec) return;

          write_handler(bytes_transferred, m, *this);
        };

        connection = std::make_unique<Connection<message::Message>>(
            socket_, conn_read_handler, conn_write_handler);
        connect_handler(endpoint, *this);
      });
}

void Client::write(message::Message m) { connection->write(m); }

void Client::poll() { io_context_.poll(); }
