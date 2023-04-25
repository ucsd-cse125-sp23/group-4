#include <boost/asio.hpp>
#include <config/lib.hpp>
#include <iostream>
#include <network/tcp_server.hpp>

int main(int argc, char *argv[]) {
<<<<<<< HEAD
  auto config = get_config();

  boost::asio::io_context io_context;
  TCPServer server(io_context, config["server_port"]);
  io_context.run();

=======
  //test_core("server");
  test_network("server");
  std::cout << "server is running!" << std::endl;
>>>>>>> 316ee1c (Partial-tested Game Engine)
  return 0;
}
