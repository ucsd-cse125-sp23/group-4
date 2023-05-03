#include <iostream>
#include <memory>
#include <network/message.hpp>
#include <network/tcp_client.hpp>

TCPClient::TCPClient(boost::asio::io_context& io_context, std::string host,
                     std::string port)
    : socket_(io_context) {
  tcp::resolver resolver(io_context);
  boost::asio::async_connect(
      socket_, resolver.resolve(host, port),
      [&](boost::system::error_code ec, tcp::endpoint endpoint) {
        if (ec) {
          std::cerr << "Error: " << ec.message() << std::endl;
          return;
        }
        std::cout << "Connected to " << endpoint.address() << ":"
                  << endpoint.port() << std::endl;

        auto read_handler = [&](boost::system::error_code ec,
                                const message::Message& m) {
          if (ec) {
            std::cerr << "Error while reading: " << ec.message() << std::endl;
          }
        };
        auto write_handler = [&](boost::system::error_code ec,
                                 std::size_t length) {
          if (ec) {
            std::cerr << "Error while writing: " << ec.message() << std::endl;
          }

          std::cout << "Wrote " << length << " bytes to server" << std::endl;
        };

        connection = std::make_unique<Connection<message::Message>>(
            socket_, read_handler, write_handler);

        message::GreetingBody g = {"Hello!"};
        message::Message m = {
            message::Type::Greeting, {1, std::time(nullptr)}, g};
        write(m);
      });
}

void TCPClient::read() { connection->read(); }

void TCPClient::write(message::Message message) { connection->write(message); }
