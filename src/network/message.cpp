#include <boost/functional/overloaded_function.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/variant.hpp>
#include <ctime>
#include <magic_enum.hpp>
#include <network/message.hpp>

namespace message {

std::string Message::to_string() const {
  std::string body_str =
      boost::apply_visitor([](auto b) { return b.to_string(); }, body);

  char buffer[80];
  std::strftime(buffer, sizeof(buffer), "%m-%d-%Y %I:%M:%S %p",
                std::localtime(&metadata.time));

  // clang-format off
  std::string str =
      std::string("") +
      "{" +                                                             "\n"
      "  type: " + std::string(magic_enum::enum_name(type)) + "," +     "\n"
      "  metadata: {," +                                                "\n"
      "    client_id: " + boost::uuids::to_string(metadata.id) + "," + "\n"
      "    time: " + buffer + "," +              "\n"
      "  }," +                                                          "\n"
      "  body: {" +                                                     "\n"
      + body_str +                                                      "\n"
      "  }" +                                                           "\n"
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
  return "    assigned_pid: " + std::to_string(pid);
}

std::string Greeting::to_string() const { return "    greeting: " + greeting; }

std::string Notify::to_string() const { return "    message: " + message; }

std::string GameStateUpdateItem::to_string() const {
  // clang-format off
    std::string str = std::string("") +
      "      {"                                       "\n"
      "        id: " + std::to_string(id) + "," +     "\n"
      "        position: {" +                         "\n"
      "          " + std::to_string(posx) + "," +     "\n"
      "          " + std::to_string(posy) + "," +     "\n"
      "          " + std::to_string(posz) + "," +     "\n"
      "        }," +                                  "\n"
      "        heading: " + std::to_string(heading) + "\n"
      "      },"                                      "\n";
  // clang-format on
  return str;
}

std::string GameStateUpdate::to_string() const {
  std::string str = "    game_things: [\n";
  for (auto& [_, thing] : things) {
    str += thing.to_string();
  }
  str += "    ]";

  return str;
}

std::string UserStateUpdate::to_string() const {
  // clang-format off
    std::string str = std::string("") +
      "      id: " + std::to_string(id) + "," +     "\n"
      "      movement_delta: {" +                   "\n"
      "        " + std::to_string(movx) + "," +     "\n"
      "        " + std::to_string(movy) + "," +     "\n"
      "        " + std::to_string(movz) + "," +     "\n"
      "      }," +                                  "\n"
      "      jump: " + std::to_string(jump) +       "\n"
      "      heading: " + std::to_string(heading) + "\n";
  // clang-format on
  return str;
}

}  // namespace message
