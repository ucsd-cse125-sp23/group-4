#pragma once

#include <array>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>

using boost::asio::ip::tcp;

template <typename T>
class Connection {
 public:
  using ReadHandler =
      std::function<void(boost::system::error_code ec, const T& t)>;
  using WriteHandler = std::function<void(boost::system::error_code ec,
                                          std::size_t length, const T& t)>;

  Connection(tcp::socket& s, ReadHandler rh, WriteHandler wh)
      : socket_(std::move(s)), read_handler(rh), write_handler(wh) {}

  ~Connection() {
    std::cout << "(~Connection) Closing connection and its socket" << std::endl;
    socket_.close();
  }

  void start();
  void read();
  void write(const T&);

 private:
  using Payload = std::pair<std::string, std::string>;

  std::size_t parse_header();
  T parse_data();
  std::string build_header(const std::string&);
  std::string serialize_data(const T&);

  tcp::socket socket_;
  const int header_length_ = 8;
  std::array<char, 8> inbound_header_;
  std::vector<char> inbound_data_;
  std::list<Payload> outbound_payloads_;
  ReadHandler read_handler;
  WriteHandler write_handler;
};

template <typename T>
void Connection<T>::start() {
  read();
}

template <typename T>
std::size_t Connection<T>::parse_header() {
  std::istringstream is(
      std::string(inbound_header_.begin(), inbound_header_.end()));
  std::size_t inbound_data_size = 0;
  if (!(is >> std::hex >> inbound_data_size)) {
    throw boost::system::error_code(boost::asio::error::invalid_argument);
  }
  return inbound_data_size;
}

template <typename T>
T Connection<T>::parse_data() {
  try {
    T t;
    std::string archive_data(&inbound_data_[0], inbound_data_.size());
    std::istringstream archive_stream(archive_data);
    boost::archive::text_iarchive archive(archive_stream);
    archive >> t;
    return t;
  } catch (const std::exception& e) {
    throw boost::system::error_code(boost::asio::error::invalid_argument);
  }
}

template <typename T>
std::string Connection<T>::serialize_data(const T& data) {
  std::ostringstream archive_stream;
  boost::archive::text_oarchive archive(archive_stream);
  archive << data;
  return archive_stream.str();
}

template <typename T>
std::string Connection<T>::build_header(const std::string& outbound_data) {
  std::ostringstream header_stream;
  header_stream << std::setw(header_length_) << std::hex
                << outbound_data.size();

  if (!header_stream || header_stream.str().size() != header_length_) {
    throw boost::system::error_code(boost::asio::error::invalid_argument);
  }

  return header_stream.str();
}

template <typename T>
void Connection<T>::read() {
  // read header
  boost::asio::async_read(
      socket_, boost::asio::buffer(inbound_header_),
      [&](boost::system::error_code header_ec, std::size_t header_bytes_read) {
        if (header_ec) {
          return read_handler(header_ec, T());
        }

        try {
          std::size_t inbound_data_size = parse_header();
          inbound_data_.resize(inbound_data_size);

          // read message
          boost::system::error_code message_ec;
          std::size_t message_bytes_read = boost::asio::read(
              socket_, boost::asio::buffer(inbound_data_), message_ec);
          if (message_ec) {
            return read_handler(message_ec, T());
          }

          T t = parse_data();
          read_handler(message_ec, t);
        } catch (const boost::system::error_code& e) {
          read_handler(e, T());
        }
      });
}

template <typename T>
void Connection<T>::write(const T& data) {
  try {
    std::string outbound_data = serialize_data(data);
    std::string outbound_header = build_header(outbound_data);
    outbound_payloads_.push_back({outbound_header, outbound_data});
  } catch (const boost::system::error_code& ec) {
    return write_handler(ec, 0, data);
  }

  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(boost::asio::buffer(outbound_payloads_.back().first));
  buffers.push_back(boost::asio::buffer(outbound_payloads_.back().second));
  boost::asio::async_write(
      socket_, buffers, [&](boost::system::error_code ec, std::size_t length) {
        outbound_payloads_.pop_front();
        write_handler(ec, length, data);
      });
}
