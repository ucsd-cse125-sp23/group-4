#include <network/message.hpp>

std::string message::Message::to_string() const {
  std::string str =
      std::string("Message {\n") + "  type: " + std::to_string(type) + ",\n" +
      "  metadata: {,\n" +
      "    player_id: " + std::to_string(metadata.player_id) + ",\n" +
      "    time: " + std::to_string(metadata.time) + ",\n" + "  }, \n" +
      "  body: {\n" + "    greeting: " + body.greeting + "\n" + "  }\n" + "}";
  return str;
}

std::ostream& message::operator<<(std::ostream& os, const message::Message& m) {
  return os << m.to_string();
}
