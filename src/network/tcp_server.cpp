#include <algorithm>
#include <boost/bind/bind.hpp>
#include <boost/functional/overloaded_function.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/variant.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <magic_enum.hpp>
#include <map>
#include <memory>
#include <network/message.hpp>
#include <network/tcp_server.hpp>
#include <numeric>
#include <string>
#include <vector>

#include "client/graphics/ColliderImporter.h"
#include "core/lib.hpp"

Server::Server(boost::asio::io_context& io_context, int port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      timer_(boost::asio::steady_timer(io_context)),
      game_(Game()) {
  std::cout << "(Server) Server running on port " << port << std::endl;

  do_accept();
  tick();
}

void Server::tick() {
  auto prev_time = std::chrono::steady_clock::now();
  timer_.expires_from_now(TICK_RATE);
  timer_.async_wait([=](const boost::system::error_code& ec) {
    if (ec) {
      std::cerr << "(Server::tick) Error waiting for timer: " << ec.message()
                << std::endl;
      return;
    }

    auto curr_time = std::chrono::steady_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        curr_time - prev_time);
    std::cout << "(Server::tick) Updating game, time elapsed since last tick: "
              << time_elapsed.count() << "ms" << std::endl;

    // advance game, send update to client
    game_.tick();
    auto things = game_.to_network();
    write_all<message::GameStateUpdate>(things);

    tick();
  });
}

void Server::do_accept() {
  // handler on new client connections
  auto accept_handler = [=](boost::system::error_code ec, tcp::socket socket) {
    if (ec) {
      std::cerr << "(Server::accept) Error accepting connection: "
                << ec.message() << std::endl;
      return;
    }

    do_accept();  // start accepting new connections

    // generate new player_id
    ClientID new_client_id = boost::uuids::random_generator()();
    std::cout << "(Server::accept) Accepted new client, assigning player_id: "
              << new_client_id << std::endl;

    auto conn_read_handler = [=](boost::system::error_code ec,
                                 const message::Message& m) {
      if (ec) {
        if (ec == boost::asio::error::eof) {
          std::cout << "(Server::read) Player " << new_client_id
                    << " disconnected, closing connection" << std::endl;
          // save value of this, since read_handler closure is destroyed when
          // connection is destroyed
          auto saved_this = this;
          connections_.erase(new_client_id);
          std::cout << saved_this << std::endl;
        }
        return;
      }

      ClientID client_id = m.metadata.id;
      auto greeting_handler = [&](const message::Greeting& body) {
        std::string greeting =
            "Hello, client " + boost::uuids::to_string(client_id) + "!";
        write<message::Greeting>(client_id, greeting);
      };
      auto user_state_update_handler =
          [&](const message::UserStateUpdate& body) { game_.update(body); };
      auto any_handler = [](const message::Message::Body&) {};

      auto message_handler = boost::make_overloaded_function(
          greeting_handler, user_state_update_handler, any_handler);
      boost::apply_visitor(message_handler, m.body);

      read(client_id);
    };

    auto conn_write_handler = [=](boost::system::error_code ec,
                                  std::size_t length,
                                  const message::Message& m) {
      if (ec) return;
    };

    // store new connection
    connections_.insert(
        {new_client_id, std::make_unique<Connection<message::Message>>(
                            socket, conn_read_handler, conn_write_handler)});
    auto& connection = connections_[new_client_id];
    std::cout << this << std::endl;

    // assign client their player_id
    int new_player_id = game_.create_player();
    write<message::Assign>(new_client_id, new_player_id);

    // notify everyone a new player has joined
    // TODO: don't notify newly joined client
    std::string notification =
        "Client " + boost::uuids::to_string(new_client_id) + " has joined";
    write_all<message::Notify>(notification);

    connection->start();  // start reading from client
  };

  std::cout << "(Server::do_accept) Accepting new connections" << std::endl;
  acceptor_.async_accept(accept_handler);
}

void Server::read(const ClientID& id) { connections_[id]->read(); }

void Server::write(const ClientID& id, const message::Message& m) {
  // std::cout << "Queueing write to client: " << m << std::endl;

  connections_[id]->write(m);
}

void Server::write_all(message::Message& m) {
  // std::cout << "Queueing write to all clients: " << m << std::endl;
  for (auto& kv : connections_) {
    m.metadata.id = kv.first;
    kv.second->write(m);
  }
}

std::ostream& operator<<(std::ostream& os, Server* s) {
  std::size_t num_connections = s->connections_.size();
  os << "(Server) Number of active connections: " << num_connections;

  if (num_connections > 0) {
    std::string clients_msg = "\n(Server) Active clients:";
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
