#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

class Connection : std::enable_shared_from_this<Connection> {
 public:
  explicit Connection(tcp::socket);

  template <typename T>
  T start();

  template <typename T>
  T read();

  template <typename T>
  void write(const T&);

 private:
  tcp::socket socket_;
  const int header_length_ = 8;
  std::string inbound_header_;
  std::string inbound_data_;
  std::string outbound_header_;
  std::string outbound_data_;

  template <typename T, typename Handler>
  void parse_header(T&, Handler);

  template <typename T, typename Handler>
  void parse_data(T&, Handler);
};
