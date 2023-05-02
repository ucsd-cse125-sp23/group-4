#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <iostream>
#include <network/connection.hpp>

Connection::Connection(tcp::socket socket) : socket_(std::move(socket)) {}

template <typename T>
T Connection::start() {
  return read<T>();
}

template <typename T>
T Connection::read() {
  // Issue a read operation to read exactly the number of bytes in a header.
  boost::asio::async_read(
      socket_, boost::asio::buffer(inbound_header_),
      [&, this](boost::system::error_code ec) {
        if (ec) {
          // handler(ec);
          return;
        }

        std::istringstream is(std::string(inbound_header_, header_length_));
        std::size_t inbound_data_size = 0;
        if (!(is >> std::hex >> inbound_data_size)) {
          // Header doesn't seem to be valid. Inform the caller.
          boost::system::error_code error(boost::asio::error::invalid_argument);
          // handler(error);
          return;
        }

        // Start an asynchronous call to receive the data.
        inbound_data_.resize(inbound_data_size);
        boost::asio::async_read(
            socket_, boost::asio::buffer(inbound_data_),
            [&, this](boost::system::error_code ec) {
              if (ec) {
                // handler(ec);
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
                // handler(error);
                return;
              }

              return t;
            });
      });
}

template <typename T>
void Connection::write(const T& t) {
  std::ostringstream archive_stream;
  boost::archive::text_oarchive archive(archive_stream);
  archive << t;
  outbound_data_ = archive_stream.str();

  std::ostringstream header_stream;
  header_stream << std::setw(header_length_) << std::hex
                << outbound_data_.size();
  if (!header_stream || header_stream.str().size() != header_length_) {
    // Something went wrong, inform the caller.
    boost::system::error_code error(boost::asio::error::invalid_argument);
    // boost::asio::post(socket_.get_executor(), std::bind(handler, error));
    return;
  }
  outbound_header_ = header_stream.str();

  // Write the serialized data to the socket. We use "gather-write" to send
  // both the header and the data in a single write operation.
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(boost::asio::buffer(outbound_header_));
  buffers.push_back(boost::asio::buffer(outbound_data_));
  boost::asio::async_write(socket_, buffers, []() {});
}
