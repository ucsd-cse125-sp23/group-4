#include <iostream>
#include <network/message.hpp>
#include <network/tcp_server.hpp>

TCPServer::TCPServer(boost::asio::io_context& io_context, int port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
  std::cout << "Server running on port " << port << std::endl;
  do_accept();
}

void TCPServer::do_accept() {
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
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

          std::cout << "Received:\n" << m << std::endl;
        };

        auto write_handler = [&](boost::system::error_code ec,
                                 std::size_t length) {};

        auto connection = std::make_shared<Connection<message::Message>>(
            socket, read_handler, write_handler);
        connections_.push_back(connection);
        connection->start();

        do_accept();
      });
}
