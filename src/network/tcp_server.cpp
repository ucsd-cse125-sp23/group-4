#include <algorithm>
#include <boost/bind/bind.hpp>
#include <boost/functional/overloaded_function.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/variant.hpp>
#include <ctime>
#include <iostream>
#include <magic_enum.hpp>
#include <memory>
#include <network/message.hpp>
#include <network/tcp_server.hpp>
#include <numeric>
#include <string>

Server::Server(boost::asio::io_context& io_context, int port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
  std::cout << "(TCPServer) Server running on port " << port << std::endl;
  do_accept();
}

void Server::do_accept() {
  acceptor_.async_accept([=](boost::system::error_code ec, tcp::socket socket) {
    if (ec) {
      std::cerr << "(TCPServer::accept) Error: " << ec.message() << std::endl;
      return;
    }

    PlayerID player_id = boost::uuids::random_generator()();
    std::cout
        << "(TCPServer::accept) Accepted new client, assigning player_id: "
        << player_id << std::endl;

    auto read_handler = [=](boost::system::error_code ec,
                            const message::Message& m) {
      if (ec) {
        std::cerr << "(Connection::read) Error: " << ec.message() << std::endl;
        if (ec == boost::asio::error::eof) {
          std::cout << "(Connection::read) Player " << player_id
                    << " disconnected, closing connection" << std::endl;
          // save value of this, since read_handler closure is destroyed when
          // connection is destroyed
          auto saved_this = this;
          connections_.erase(player_id);
          std::cout << saved_this << std::endl;
        }
        return;
      }

      std::cout << "(Connection::read) Received:\n" << m << std::endl;

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
      auto notify_handler = [&](const message::Notify& body) {};

      auto message_handler = boost::make_overloaded_function(
          assign_handler, greeting_handler, notify_handler);
      boost::apply_visitor(message_handler, m.body);

      read(player_id);
    };

    auto write_handler = [=](boost::system::error_code ec, std::size_t length,
                             const message::Message& m) {
      if (ec) {
        std::cerr << "(Connection::write, " << magic_enum::enum_name(m.type)
                  << ") Error: " << ec.message() << std::endl;
        return;
      }

      std::cout << "(Connection::write, " << magic_enum::enum_name(m.type)
                << ") Successfully wrote " << length << " bytes to client "
                << player_id << std::endl;
    };

    connections_.insert(
        {player_id, std::make_unique<Connection<message::Message>>(
                        socket, read_handler, write_handler)});
    auto& connection = connections_[player_id];

    // assign client player_id
    message::Message new_m{message::Type::Assign,
                           {player_id, std::time(nullptr)},
                           message::Assign{}};
    connection->write(new_m);

    // notify everyone a new player has joined
    message::Message join_notification{
        message::Type::Notify,
        {player_id, std::time(nullptr)},
        message::Notify{"Player " + boost::uuids::to_string(player_id) +
                        " has joined"}};
    // TODO: don't notify newly joined client
    write_all(join_notification);

    connection->start();

    std::cout << this << std::endl;

    do_accept();
  });
}

void Server::read(const PlayerID& id) { connections_[id]->read(); }

void Server::write(const message::Message& m, const PlayerID& id) {
  // std::cout << "Queueing write to client: " << m << std::endl;
  connections_[id]->write(m);
}

void Server::write_all(const message::Message& m) {
  // std::cout << "Queueing write to all clients: " << m << std::endl;
  for (auto& kv : connections_) kv.second->write(m);
}

std::ostream& operator<<(std::ostream& os, Server* s) {
  std::size_t num_connections = s->connections_.size();
  os << "(TCPServer) Number of active connections: " << num_connections;

  if (num_connections > 0) {
    std::string clients_msg = "\n(TCPServer) Active clients:";
    std::string clients = std::accumulate(
        std::next(s->connections_.begin()), s->connections_.end(),
        "  " + to_string(s->connections_.begin()->first),
        [](const std::string& acc, auto& it) {
          return acc + "\n  " + to_string(it.first);
        });
    os << clients_msg << "\n" << clients;
  }

  return os;
}
