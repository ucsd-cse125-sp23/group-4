#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using boost::asio::ip::tcp;

template <typename T>
class Connection : std::enable_shared_from_this<Connection<T>> {
 public:
  using ReadHandler =
      std::function<void(boost::system::error_code ec, const T& t)>;
  using WriteHandler =
      std::function<void(boost::system::error_code ec, std::size_t length)>;

  Connection(tcp::socket& s, ReadHandler rh, WriteHandler wh)
      : socket_(std::move(s)),
        read_handler(rh),
        write_handler(wh) {}

  void start();
  void read();
  void write(const T&);

 private:
  tcp::socket socket_;
  const int header_length_ = 8;
  char inbound_header_[8];
  std::vector<char> inbound_data_;
  std::string outbound_header_;
  std::string outbound_data_;
  ReadHandler read_handler;
  WriteHandler write_handler;
};

template <typename T>
void Connection<T>::start() {
  read();
}

template <typename T>
void Connection<T>::read() {
  // Issue a read operation to read exactly the number of bytes in a header.
  boost::asio::async_read(
      socket_, boost::asio::buffer(inbound_header_),
      [&](boost::system::error_code ec, std::size_t length) {
        if (ec) {
          read_handler(ec, T());
          return;
        }
        std::cout << "Received message" << std::endl;

        std::istringstream is(std::string(inbound_header_, header_length_));
        std::size_t inbound_data_size = 0;
        if (!(is >> std::hex >> inbound_data_size)) {
          // Header doesn't seem to be valid. Inform the caller.
          boost::system::error_code error(boost::asio::error::invalid_argument);
          read_handler(ec, T());
          return;
        }

        std::cout << "Received header:\n" << inbound_data_size << std::endl;

        // Start an asynchronous call to receive the data.
        inbound_data_.resize(inbound_data_size);
        boost::asio::async_read(
            socket_, boost::asio::buffer(inbound_data_),
            [&](boost::system::error_code ec, std::size_t length) {
              if (ec) {
                read_handler(ec, T());
                return;
              }

              T t;
              try {
                std::string archive_data(&inbound_data_[0],
                                         inbound_data_.size());
                std::istringstream archive_stream(archive_data);
                boost::archive::text_iarchive archive(archive_stream);

                archive >> t;
              } catch (std::exception& e) {
                // Unable to decode data.
                boost::system::error_code error(
                    boost::asio::error::invalid_argument);
                read_handler(ec, T());
                return;
              }

              read_handler(ec, t);
            });
      });
}

template <typename T>
void Connection<T>::write(const T& t) {
  std::ostringstream archive_stream;
  boost::archive::text_oarchive archive(archive_stream);
  archive << t;
  outbound_data_ = archive_stream.str();

  std::ostringstream header_stream;
  header_stream << std::setw(header_length_) << std::hex
                << outbound_data_.size();

  if (!header_stream || header_stream.str().size() != header_length_) {
    boost::system::error_code ec(boost::asio::error::invalid_argument);
    write_handler(ec, 0);
    return;
  }
  outbound_header_ = header_stream.str();

  // Write the serialized data to the socket. We use "gather-write" to send
  // both the header and the data in a single write operation.
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(boost::asio::buffer(outbound_header_));
  buffers.push_back(boost::asio::buffer(outbound_data_));
  boost::asio::async_write(socket_, buffers, write_handler);
}
