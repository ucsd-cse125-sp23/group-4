#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <network/message.hpp>
#include <network/tcp_server.hpp>
#include <numeric>
#include <string>

Server::Server(boost::asio::io_context& io_context, int port,
               AcceptHandler accept_handler, ReadHandler read_handler,
               WriteHandler write_handler, TickHandler tick_handler)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      timer_(boost::asio::steady_timer(io_context)),
      accept_handler_(accept_handler),
      read_handler_(read_handler),
      write_handler_(write_handler),
      tick_handler_(tick_handler) {
  std::cout << "(Server::Server) Server running on port " << port << std::endl;
  do_accept();
  tick();
}

void Server::tick() {
  auto prev_time = std::chrono::steady_clock::now();
  timer_.expires_from_now(TICK_RATE);
  timer_.async_wait([this, prev_time](const boost::system::error_code& ec) {
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

    tick_handler_(*this);
    tick();
  });
}

void Server::do_accept() {
  // handler on new client connections
  auto accept_handler = [this](boost::system::error_code ec,
                               tcp::socket socket) {
    if (ec) {
      std::cerr << "(Server::accept) Error accepting connection: "
                << ec.message() << std::endl;
      return;
    }

    do_accept();  // start accepting new connections

    // generate id for new client
    ClientID new_client_id = boost::uuids::random_generator()();
    std::cout << "(Server::accept) Accepted new client, assigning client_id: "
              << new_client_id << std::endl;

    auto conn_read_handler = [this, new_client_id](boost::system::error_code ec,
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

      read_handler_(m, *this);
      read(m.metadata.id);
    };

    auto conn_write_handler = [this](boost::system::error_code ec,
                                     std::size_t length,
                                     const message::Message& m) {
      if (ec) return;

      write_handler_(length, m, *this);
    };

    // store new connection
    connections_.insert(
        {new_client_id, std::make_unique<Connection<message::Message>>(
                            socket, conn_read_handler, conn_write_handler)});
    auto& connection = connections_[new_client_id];
    std::cout << this << std::endl;

    accept_handler_(new_client_id, *this);
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
