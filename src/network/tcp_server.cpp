#include <algorithm>
#include <boost/bind/bind.hpp>
#include <boost/functional/overloaded_function.hpp>
#include <boost/variant.hpp>
#include <ctime>
#include <iostream>
#include <network/message.hpp>
#include <network/tcp_server.hpp>
#include <string>

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

    int connection_idx = connections_.size();
    std::string player_id = std::to_string(connection_idx);

    std::cout << "Accepted new client, assigning player_id " << player_id
              << std::endl;

    auto read_handler = [=](boost::system::error_code ec,
                            const message::Message& m) {
      if (ec) {
        std::cerr << "(read) Error: " << ec.message() << std::endl;
        return;
      }

      std::cout << "Received " << m << std::endl;

      std::string player_id = m.metadata.player_id;
      auto assign_handler = [&](const message::Assign& body) {};
      auto greeting_handler = [&](const message::Greeting& body) {
        message::Message new_m{message::Type::Greeting,
                               {player_id, std::time(nullptr)},
                               message::Greeting{"Hello, player " + player_id}};
        write(new_m, connection_idx);
      };

      auto message_handler = boost::bind<void>(
          boost::make_overloaded_function(assign_handler, greeting_handler),
          boost::placeholders::_1);

      boost::apply_visitor(message_handler, m.body);

      read(connection_idx);
    };

    auto write_handler = [=](boost::system::error_code ec, std::size_t length) {
      if (ec) {
        std::cerr << "(write) Error: " << ec.message() << std::endl;
        return;
      }

      std::cout << "Successfully wrote " << length << " bytes to client "
                << player_id << std::endl;
    };

    auto connection = std::make_shared<Connection<message::Message>>(
        socket, read_handler, write_handler);
    message::Message new_m{message::Type::Assign,
                           {player_id, std::time(nullptr)},
                           message::Assign{}};
    connection->write(new_m);

    // notify everyone a new player has joined
    message::Message join_notification{
        message::Type::Greeting,
        {player_id, std::time(nullptr)},
        message::Greeting{"Player " + player_id + " has joined"}};
    write_all(join_notification);

    connections_.push_back(connection);
    connection->start();

    do_accept();
  });
}

void TCPServer::read(int idx) { connections_[idx]->read(); }

void TCPServer::write(const message::Message& m, int idx) {
  // std::cout << "Queueing write to client: " << m << std::endl;
  connections_[idx]->write(m);
}

void TCPServer::write_all(const message::Message& m) {
  // std::cout << "Queueing write to all clients: " << m << std::endl;
  for (auto& c : connections_) c->write(m);
}
