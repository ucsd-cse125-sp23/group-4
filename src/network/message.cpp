#include <network/message.hpp>

std::ostream& operator<<(std::ostream& os, const message::Message& m) {
  return os << "Message: {type: " << m.type << "}";
}
