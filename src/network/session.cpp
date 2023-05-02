#include <boost/asio.hpp>
#include <iostream>
#include <network/session.hpp>

Session::Session(tcp::socket socket) : socket_(std::move(socket)) {}

void Session::start() { read(); }

void Session::read() {
  socket_.async_read_some(
      boost::asio::buffer(data_),
      [this](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          std::cout << "read " << length << " bytes" << std::endl;
          std::cout << "message: " << data_ << std::endl;
          write(length);
        }
      });
}

void Session::write(std::size_t length) {
  boost::asio::async_write(
      socket_, boost::asio::buffer(data_, length),
      [this](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          read();
        }
      });
}
