#include <boost/asio.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <network/message.hpp>
#include <network/tcp_server.hpp>
#include <numeric>
#include <string>

Server::Server(int port, AcceptHandler accept_handler,
               CloseHandler close_handler, ReadHandler read_handler,
               WriteHandler write_handler, TickHandler tick_handler)
    : io_context_(boost::asio::io_context()),
      acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)),
      timer_(boost::asio::steady_timer(io_context_)),
      accept_handler_(accept_handler),
      close_handler_(close_handler),
      read_handler_(read_handler),
      write_handler_(write_handler),
      tick_handler_(tick_handler) {
  std::cout << "(Server::Server) Server running on port " << port << std::endl;
  do_accept();
  tick();
  io_context_.run();
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
    ClientID client_id = boost::uuids::random_generator()();
    std::cout << "(Server::accept) Accepted new client, assigning client_id: "
              << client_id << std::endl;

    auto conn_read_handler = [this, client_id](boost::system::error_code ec,
                                               const message::Message& m) {
      if (ec) {
        if (ec == boost::asio::error::eof ||
            ec == boost::asio::error::connection_reset) {
          std::cout << "(Server::read) Player " << client_id
                    << " disconnected, closing connection" << std::endl;
          close_handler_(client_id, *this);
          // save value of this, since read_handler closure is destroyed when
          // connection is destroyed
          auto saved_this = this;
          connections_.erase(client_id);
          std::cout << saved_this << std::endl;
        }
        return;
      }

      read_handler_(m, *this);
    };

    auto conn_write_handler = [this](boost::system::error_code ec,
                                     std::size_t length,
                                     const message::Message& m) {
      if (ec) return;

      write_handler_(length, m, *this);
    };

    // store new connection
    connections_.insert(
        {client_id, std::make_unique<Connection<message::Message>>(
                        socket, conn_read_handler, conn_write_handler)});
    accept_handler_(client_id, *this);
    std::cout << this << std::endl;  // print Server status
  };

  std::cout << "(Server::do_accept) Accepting new connections" << std::endl;
  acceptor_.async_accept(accept_handler);
}

void Server::write(const ClientID& id, const message::Message& m) {
  connections_[id]->write(m);
}

void Server::write_all(message::Message& m) {
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
