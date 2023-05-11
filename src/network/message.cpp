#include <boost/variant.hpp>
#include <magic_enum.hpp>
#include <network/message.hpp>

#include "boost/uuid/uuid_io.hpp"

namespace message {

std::string Message::toString() const {
  std::string body_str =
      boost::apply_visitor([](auto b) { return b.toString(); }, body);

  // clang-format off
  std::string str =
      std::string("") +
      "Message {" +                                                             "\n" // NOLINT
      "  type: " + std::string(magic_enum::enum_name(type)) + "," +             "\n" // NOLINT
      "  metadata: {," +                                                        "\n" // NOLINT
      "    player_id: " + boost::uuids::to_string(metadata.player_id) + "," +   "\n" // NOLINT
      "    time: " + std::to_string(metadata.time) + "," +                      "\n" // NOLINT
      "  }," +                                                                  "\n" // NOLINT
      "  body: {" +                                                             "\n" // NOLINT
      "    " + body_str +                                                       "\n" // NOLINT
      "  }" +                                                                   "\n" // NOLINT
      "}";
  // clang-format on
  return str;
}

std::ostream& operator<<(std::ostream& os, const message::Message& m) {
  return os << m.toString();
}

}  // namespace message
