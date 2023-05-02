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
        if (!ec) {
          auto connection = std::make_shared<Connection>(std::move(socket));
          connections_.push_back(connection);
          message::Message m = connection->start<message::Message>();
          std::cout << "Received " << m << std::endl;
        }

        do_accept();
      });
}
