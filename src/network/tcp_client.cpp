#include <network/message.hpp>
#include <network/tcp_client.hpp>

TCPClient::TCPClient(boost::asio::io_context& io_context, std::string host,
                     std::string port)
    : socket_(io_context), connection(Connection(tcp::socket(io_context))) {
  tcp::resolver resolver(io_context);
  boost::asio::connect(socket_, resolver.resolve(host, port));
}

message::Message TCPClient::read() {
  return connection.read<message::Message>();
}

void TCPClient::write(message::Message message) { connection.write(message); }
