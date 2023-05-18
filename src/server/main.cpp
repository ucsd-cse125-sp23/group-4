#include <boost/asio.hpp>
#include <config/lib.hpp>
#include <network/tcp_server.hpp>

int main(int argc, char *argv[]) {
  auto config = get_config();

  boost::asio::io_context io_context;
  Server server(io_context, config["server_port"]);
  io_context.run();

  return 0;
}
