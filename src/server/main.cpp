#include <iostream>
#include <network/tcp_server.hpp>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "./server <port>" << std::endl;
    return 1;
  }

  boost::asio::io_context io_context;
  TCPServer server(io_context, std::stoi(argv[1]));
  io_context.run();

  return 0;
}
