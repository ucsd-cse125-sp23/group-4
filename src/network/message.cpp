#include <boost/variant.hpp>
#include <magic_enum.hpp>
#include <network/message.hpp>

namespace message {

std::string Message::toString() const {
  std::string body_str =
      boost::apply_visitor([](auto b) { return b.toString(); }, body);

  std::string str =
      // clang-format off
      std::string("") +
      "Message {" +                                                   "\n"
      "  type: " + std::string(magic_enum::enum_name(type)) + "," +   "\n"
      "  metadata: {," +                                              "\n"
      "    player_id: " + metadata.player_id + "," +                  "\n"
      "    time: " + std::to_string(metadata.time) + "," +            "\n"
      "  }," +                                                        "\n"
      "  body: {" +                                                   "\n"
      "    " + body_str +                                             "\n"
      "  }" +                                                         "\n"
      "}";
  // clang-format on
  return str;
}

std::ostream& operator<<(std::ostream& os, const message::Message& m) {
  return os << m.toString();
}

}  // namespace message
