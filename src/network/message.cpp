#include <boost/functional/overloaded_function.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/variant.hpp>
#include <magic_enum.hpp>
#include <network/message.hpp>

namespace message {

std::string Message::to_string() const {
  std::string body_str =
      boost::apply_visitor([](auto b) { return b.to_string(); }, body);

  // clang-format off
  std::string str =
      std::string("") +
      "{" +                                                                     "\n" // NOLINT
      "  type: " + std::string(magic_enum::enum_name(type)) + "," +             "\n" // NOLINT
      "  metadata: {," +                                                        "\n" // NOLINT
      "    player_id: " + boost::uuids::to_string(metadata.pid) + "," +   "\n" // NOLINT
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
  return os << m.to_string();
}

Type get_type(const Message::Body& body) {
  auto assign = [](const Assign&) { return Type::Assign; };
  auto greeting = [](const Greeting&) { return Type::Greeting; };
  auto notify = [](const Notify&) { return Type::Notify; };
  auto gamestate = [](const GameStateUpdate&) { return Type::GameStateUpdate; };
  auto userstate = [](const UserStateUpdate&) { return Type::UserStateUpdate; };
  auto overload = boost::make_overloaded_function(assign, greeting, notify,
                                                  gamestate, userstate);
  return boost::apply_visitor(overload, body);
}

std::string Assign::to_string() const {
  return "assigning player_id: " + boost::uuids::to_string(pid);
}

std::string Greeting::to_string() const { return greeting; }

std::string Notify::to_string() const { return message; }

std::string GameStateUpdateItem::to_string() const {
  // clang-format off
    std::string str = std::string("") + "{\n" +
                      "  id: " + std::to_string(id) +
                      ",\n" +
                      "  position: {\n" +
                      "  " + std::to_string(posx) +
                      ", " + std::to_string(posy) +
                      ", " + std::to_string(posz) +
                      "\n" +
                      "  },\n" +
                      "  heading: " + std::to_string(heading) +
                      ",\n" +
                      "}";
  // clang-format on
  return str;
}

std::string GameStateUpdate::to_string() const {
  // clang-format off
    std::string str = std::string("") + "{...\n" +
                      "game things:\n";
  // clang-format on
  for (auto i : things) {
    str += i.to_string();
  }
  str += "\n}...\n";
  return str;
}

std::string UserStateUpdate::to_string() const {
  // clang-format off
    std::string str = std::string("") + "{\n" +
                      "  id: " + std::to_string(id) +
                      ",\n" +
                      "  movement delta: {\n" +
                      "  " + std::to_string(movx) +
                      ", " + std::to_string(movy) +
                      ", " + std::to_string(movz) +
                      "\n" +
                      "  },\n" +
                      "  jump: " + std::to_string(jump) +
                      ",\n" +
                      "  heading: " + std::to_string(heading) +
                      ",\n" +
                      "}";
  // clang-format on
  return str;
}

}  // namespace message
