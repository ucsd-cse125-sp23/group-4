#include <ctime>
#include <iostream>
#include <network/message.hpp>
#include <network/tcp_server.hpp>

TCPServer::TCPServer(boost::asio::io_context& io_context, int port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
  std::cout << "Server running on port " << port << std::endl;
  do_accept();
}

void TCPServer::do_accept() {
  acceptor_.async_accept([&](boost::system::error_code ec, tcp::socket socket) {
    if (ec) {
      std::cerr << "(accept) Error: " << ec.message() << std::endl;
      return;
    }

    auto read_handler = [&](boost::system::error_code ec,
                            const message::Message& m) {
      if (ec) {
        std::cerr << "(read) Error: " << ec.message() << std::endl;
        return;
      }

      std::cout << "Received " << m << std::endl;

      message::Greeting g = {"Hello player " +
                             std::to_string(m.metadata.player_id) + "!"};
      message::Message new_message = {
          message::Type::Greeting, {1, std::time(nullptr)}, g};
      write(new_message);
    };

    auto write_handler = [&](boost::system::error_code ec, std::size_t length) {
      if (ec) {
        std::cerr << "(write) Error: " << ec.message() << std::endl;
        return;
      }

      std::cout << "Successfully wrote " << length << " bytes to client"
                << std::endl;
    };

    std::cout << "Connection established with client" << std::endl;
    auto connection = std::make_shared<Connection<message::Message>>(
        socket, read_handler, write_handler);
    connections_.push_back(connection);
    connection->start();

    do_accept();
  });
}

void TCPServer::write(const message::Message& m) {
  std::cout << "Queueing write to client: " << m << std::endl;
  connections_.back()->write(m);
}
