#include <algorithm>
#include <boost/bind/bind.hpp>
#include <boost/functional/overloaded_function.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/variant.hpp>
#include <ctime>
#include <iostream>
#include <network/message.hpp>
#include <network/tcp_server.hpp>
#include <string>

Server::Server(boost::asio::io_context& io_context, int port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
  std::cout << "Server running on port " << port << std::endl;
  do_accept();
}

void Server::do_accept() {
  acceptor_.async_accept([&](boost::system::error_code ec, tcp::socket socket) {
    if (ec) {
      std::cerr << "(accept) Error: " << ec.message() << std::endl;
      return;
    }

    PlayerID player_id = boost::uuids::random_generator()();

    std::cout << "Accepted new client, assigning player_id " << player_id
              << std::endl;

    auto read_handler = [=](boost::system::error_code ec,
                            const message::Message& m) {
      if (ec) {
        std::cerr << "(read) Error: " << ec.message() << std::endl;
        if (ec == boost::asio::error::eof) {
          connections_.erase(player_id);
        }
        return;
      }

      std::cout << "Received " << m << std::endl;

      PlayerID player_id = m.metadata.player_id;
      auto assign_handler = [&](const message::Assign& body) {};
      auto greeting_handler = [&](const message::Greeting& body) {
        message::Message new_m{
            message::Type::Greeting,
            {player_id, std::time(nullptr)},
            message::Greeting{"Hello, player " +
                              boost::uuids::to_string(player_id)}};
        write(new_m, player_id);
      };

      auto message_handler = boost::bind<void>(
          boost::make_overloaded_function(assign_handler, greeting_handler),
          boost::placeholders::_1);

      boost::apply_visitor(message_handler, m.body);

      read(player_id);
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

    // assign client player_id
    message::Message new_m{message::Type::Assign,
                           {player_id, std::time(nullptr)},
                           message::Assign{}};
    connection->write(new_m);

    // notify everyone a new player has joined
    message::Message join_notification{
        message::Type::Greeting,
        {player_id, std::time(nullptr)},
        message::Greeting{"Player " + boost::uuids::to_string(player_id) +
                          " has joined"}};
    write_all(join_notification);

    connections_[player_id] = connection;
    connection->start();

    do_accept();
  });
}

void Server::read(PlayerID& id) { connections_[id]->read(); }

void Server::write(const message::Message& m, PlayerID& id) {
  std::cout << "Queueing write to client: " << m << std::endl;
  connections_[id]->write(m);
}

void Server::write_all(const message::Message& m) {
  // std::cout << "Queueing write to all clients: " << m << std::endl;
  for (auto& kv : connections_) kv.second->write(m);
}
