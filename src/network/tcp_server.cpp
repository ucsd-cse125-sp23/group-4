#include <iostream>
#include <network/session.hpp>
#include <network/tcp_server.hpp>

TCPServer::TCPServer(boost::asio::io_context& io_context, const int port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
  std::cout << "server is running!" << std::endl;
  do_accept();
}

void TCPServer::do_accept() {
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
          auto session = std::make_shared<Session>(std::move(socket));
          sessions_.push_back(session);
          session->start();
        }
        do_accept();
      });
}
