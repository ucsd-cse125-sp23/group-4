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

/* TEMP SERVER GAME STATE CODE (positions only) */

struct GameThingState {
  int id;
  float posx;
  float posy;
  float posz;
  float heading;

  GameThingState() {}

  void move(float x, float y, float z) {
    posx += x;
    posy += y;
    posz += z;
  }

  const message::GameStateUpdateItem toMessage() {
    message::GameStateUpdateItem p;
    p.id = id;
    p.posx = posx;
    p.posy = posy;
    p.posz = posz;
    p.heading = heading;

    return p;
  }
};

// stored in server
struct GameState {
  std::map<int, GameThingState> objectStates;

  GameState() {}
};

/* *** */

Server::Server(boost::asio::io_context& io_context, int port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      timer_(boost::asio::steady_timer(io_context)) {
  std::cout << "(TCPServer) Server running on port " << port << std::endl;
  do_accept();
  tick();
}

GameState server_gameState = GameState();

void Server::tick() {
  auto prev_time = std::chrono::steady_clock::now();
  timer_.expires_from_now(tick_rate_);
  timer_.async_wait([=](const boost::system::error_code& ec) {
    if (ec) {
      std::cerr << "(TCPServer::tick) Error: " << ec.message() << std::endl;
      return;
    }
    auto curr_time = std::chrono::steady_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        curr_time - prev_time);
    // std::cout
    //    << "(TCPServer::tick) Updating game, time elapsed since last tick: "
    //    << time_elapsed.count() << "ms" << std::endl;

    // Temporary server broadcast example (sending to client) ---
    update_num_++;

    std::vector<message::GameStateUpdateItem*> thingsOnServer;  // to send

    for (auto thing : server_gameState.objectStates) {
      thingsOnServer.push_back(
          new message::GameStateUpdateItem(thing.second.toMessage()));
    }

    /*
    message::GameStateUpdateItem* p = new message::GameStateUpdateItem();
    p->id = 3;  // just for testing
    p->posx = update_num_ * 0.1f;
    p->posy = 2;
    p->posz = 2;
    thingsOnServer.push_back(p);
    */

    // Temporary server loop demo (sending to client) ---
    update_num_++;

    std::vector<message::GameStateUpdateItem*> thingsOnServer;  // to send

    message::GameStateUpdateItem* p = new message::GameStateUpdateItem();
    p->id = 3;  // just for testing
    p->posx = update_num_ * 0.1f;
    p->posy = 2;
    p->posz = 2;
    thingsOnServer.push_back(p);

    message::Message game_state_update{
        message::Type::GameStateUpdate,
        {boost::uuids::random_generator()(), std::time(nullptr)},
        message::GameStateUpdate{thingsOnServer}};
    // ---

    write_all(game_state_update);

    tick();
  });
}

void Server::do_accept() {
  // handler on new client connections
  auto accept_handler = [=](boost::system::error_code ec, tcp::socket socket) {
    if (ec) {
      std::cerr << "(TCPServer::accept) Error: " << ec.message() << std::endl;
      return;
    }

    do_accept();  // start accepting new connections

    // generate new player_id
    PlayerID player_id = boost::uuids::random_generator()();
    std::cout
        << "(TCPServer::accept) Accepted new client, assigning player_id: "
        << player_id << std::endl;

    auto conn_read_handler = [=](boost::system::error_code ec,
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
      auto game_state_update_handler =
          [&](const message::GameStateUpdate& body) {};
      auto user_state_update_handler =
          [&](const message::UserStateUpdate& body) {
            int rec_id = body.id;

            // Temporary server receive example (reading from client) ---

            if (server_gameState.objectStates.find(rec_id) ==
                server_gameState.objectStates.end()) {
              // id not found
              GameThingState i;
              i.id = rec_id;
              i.posx = body.movx;
              i.posy = body.movy;
              i.posz = body.movz;
              server_gameState.objectStates[rec_id] = i;  // add to object map
            } else {
              // id found
              server_gameState.objectStates[rec_id].move(body.movx, body.movy,
                                                         body.movz);
              server_gameState.objectStates[rec_id].heading = body.heading;
            }

            // ---
          };

      auto message_handler = boost::make_overloaded_function(
          assign_handler, greeting_handler, notify_handler,
          game_state_update_handler, user_state_update_handler);
      boost::apply_visitor(message_handler, m.body);

      read(player_id);
    };

    auto conn_write_handler = [=](boost::system::error_code ec,
                                  std::size_t length,
                                  const message::Message& m) {
      if (ec) {
        std::cerr << "(Connection::write, " << magic_enum::enum_name(m.type)
                  << ") Error: " << ec.message() << std::endl;
        return;
      }

      // std::cout << "(Connection::write, " << magic_enum::enum_name(m.type)
      //          << ") Successfully wrote " << length << " bytes to client "
      //          << player_id << std::endl;
    };

    // store new connection
    connections_.insert(
        {player_id, std::make_unique<Connection<message::Message>>(
                        socket, conn_read_handler, conn_write_handler)});
    auto& connection = connections_[player_id];
    std::cout << this << std::endl;

    // assign client their player_id
    message::Message new_m{message::Type::Assign,
                           {player_id, std::time(nullptr)},
                           message::Assign{}};
    connection->write(new_m);

    // notify everyone a new player has joined
    // TODO: don't notify newly joined client
    message::Message join_notification{
        message::Type::Notify,
        {player_id, std::time(nullptr)},
        message::Notify{"Player " + boost::uuids::to_string(player_id) +
                        " has joined"}};
    write_all(join_notification);

    connection->start();  // start reading from client
  };

  acceptor_.async_accept(accept_handler);
}

void Server::read(const PlayerID& id) { connections_[id]->read(); }

void Server::write(const message::Message& m, const PlayerID& id) {
  // std::cout << "Queueing write to client: " << m << std::endl;
  connections_[id]->write(m);
}

void Server::write_all(message::Message& m) {
  // std::cout << "Queueing write to all clients: " << m << std::endl;
  for (auto& kv : connections_) {
    m.metadata.player_id = kv.first;
    kv.second->write(m);
  }
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
