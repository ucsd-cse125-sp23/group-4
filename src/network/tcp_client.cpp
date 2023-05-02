#include <network/tcp_client.hpp>

TCPClient::TCPClient(boost::asio::io_context& io_context, std::string host,
                     std::string port)
    : socket_(io_context) {
  tcp::resolver resolver(io_context);
  boost::asio::connect(socket_, resolver.resolve(host, port));
}

std::string TCPClient::read() {
  boost::asio::read(socket_, boost::asio::buffer(data, 12));
  return data;
}

void TCPClient::write(std::string data) {
  boost::asio::write(socket_, boost::asio::buffer(data));
}
