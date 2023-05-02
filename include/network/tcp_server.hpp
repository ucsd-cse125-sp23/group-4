#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <network/session.hpp>
#include <vector>

using boost::asio::ip::tcp;

class TCPServer {
 public:
  TCPServer(boost::asio::io_context& io_context, const int port);

 private:
  tcp::acceptor acceptor_;
  std::vector<std::shared_ptr<Session>> sessions_;

  void do_accept();
};
