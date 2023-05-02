#include <core/lib.hpp>
#include <iostream>
#include <network/lib.hpp>
#include <network/tcp_server.hpp>

int main(int argc, char *argv[]) {
  test_core("server");
  test_network("server");

  boost::asio::io_context io_context;
  TCPServer server(io_context, 3000);
  io_context.run();

  return 0;
}
