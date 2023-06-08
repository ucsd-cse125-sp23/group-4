#include <boost/functional/overloaded_function.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/variant.hpp>
#include <ctime>
#include <magic_enum.hpp>
#include <network/effect.hpp>
#include <network/message.hpp>
#include <string>
#include <vector>

namespace message {

std::string Message::to_string() const {
  std::string body_str =
      boost::apply_visitor([](auto b) { return b.to_string(); }, body);

  char buffer[80];
  std::strftime(buffer, sizeof(buffer), "%m-%d-%Y %I:%M:%S %p",
                std::localtime(&metadata.time));

  // clang-format off
  std::string str = std::string("") +
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
  auto game_state = [](const GameStateUpdate&) {
    return Type::GameStateUpdate;
  };
  auto user_state = [](const UserStateUpdate&) {
    return Type::UserStateUpdate;
  };
  auto lobby_update = [](const LobbyUpdate&) { return Type::LobbyUpdate; };
  auto lobby_player_update = [](const LobbyPlayerUpdate&) {
    return Type::LobbyPlayerUpdate;
  };
  auto game_start = [](const GameStart&) { return Type::GameStart; };
  auto jump_event = [](const JumpEvent&) { return Type::JumpEvent; };
  auto land_event = [](const LandEvent&) { return Type::LandEvent; };
  auto item_pickup_event = [](const ItemPickupEvent&) {
    return Type::ItemPickupEvent;
  };
  auto tag_event = [](const TagEvent&) { return Type::TagEvent; };
  auto overload = boost::make_overloaded_function(
      assign, greeting, notify, game_state, user_state, lobby_update,
      lobby_player_update, game_start, jump_event, land_event,
      item_pickup_event, tag_event);
  return boost::apply_visitor(overload, body);
}

std::string Assign::to_string() const {
  return "    assigned_pid: " + std::to_string(pid);
}

std::string Greeting::to_string() const { return "    greeting: " + greeting; }

std::string Notify::to_string() const { return "    message: " + message; }

std::string effects_to_string(const std::vector<Effect>& effects) {
  if (effects.empty()) return "[]";

  std::string inside;
  for (auto& e : effects) inside += std::string(magic_enum::enum_name(e)) + ",";

  return "[" + inside + "]";
}

std::string Player::to_string() const {
  // clang-format off
  std::string str = std::string("") +
    "      {"                                                     "\n"
    "        id: " + std::to_string(id) + "," +                   "\n"
    "        position: {" +                                       "\n"
    "          " + std::to_string(posx) + "," +                   "\n"
    "          " + std::to_string(posy) + "," +                   "\n"
    "          " + std::to_string(posz) + "," +                   "\n"
    "        }," +                                                "\n"
    "        heading: " + std::to_string(heading) + "," +         "\n"
    "        score: " + std::to_string(score) + "," +             "\n"
    "        speed: " + std::to_string(speed) + "," +             "\n"
    "        is_grounded: " + std::to_string(is_grounded) + "," + "\n"
    "        is_tagged: " + std::to_string(is_tagged) + "," +     "\n"
    "        effects: " + effects_to_string(effects) + "," +      "\n"
    "      },"                                                    "\n";
  // clang-format on
  return str;
}

std::string Item::to_string() const {
  // clang-format off
  std::string str = std::string("") +
    "      {"                                                           "\n"
    "        id: " + std::to_string(id) + "," +                         "\n"
    "        item: " + std::string(magic_enum::enum_name(item)) + "," + "\n"
    "        position: {" +                                             "\n"
    "          " + std::to_string(posx) + "," +                         "\n"
    "          " + std::to_string(posy) + "," +                         "\n"
    "          " + std::to_string(posz) + "," +                         "\n"
    "        }," +                                                      "\n"
    "      },"                                                          "\n";
  // clang-format on
  return str;
}

std::string GameStateUpdate::to_string() const {
  std::string _players = "    [\n";
  for (auto& [_, player] : players) _players += player.to_string();

  _players += "    ]";

  std::string _items = "    [\n";
  for (auto& [_, item] : items) _items += item.to_string();

  _items += "    ]";

  // clang-format off
  std::string str = std::string("") +
    "      players: " + _players + "," +                      "\n"
    "      items: " + _items + "," +                          "\n"
    "      tagged_player: " + std::to_string(tagged_player) + "\n"
    "      round_time: " + std::to_string(time_elapsed) +     "\n";
  // clang-format on

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

std::string LobbyPlayerUpdate::to_string() const {
  // clang-format off
  std::string str = std::string("") +
    "      {"                                       "\n"
    "        id: " + std::to_string(id) + "," +       "\n"
    "        skin: " + skin +                         "\n"
    "        is_ready: " + std::to_string(is_ready) + "\n"
    "      },"                                       "\n";
  // clang-format on
  return str;
}

std::string LobbyUpdate::to_string() const {
  std::string str = "    players: [\n";
  for (auto& [_, player] : players) {
    str += player.to_string();
  }
  str += "    ]";

  return str;
}

std::string GameStart::to_string() const { return "    game_start: true"; }

std::string JumpEvent::to_string() const {
  return "    pid: " + std::to_string(pid);
}

std::string LandEvent::to_string() const {
  return "    pid: " + std::to_string(pid);
}

std::string ItemPickupEvent::to_string() const {
  // clang-format off
  std::string str = std::string("") +
    "      pid: " + std::to_string(pid) + "," +     "\n"
    "      item: " + std::string(magic_enum::enum_name(item)) + "\n";
  // clang-format on
  return str;
}

std::string TagEvent::to_string() const {
  // clang-format off
  std::string str = std::string("") +
    "      tagger: " + std::to_string(tagger) + "," +     "\n"
    "      taggee: " + std::to_string(taggee) + "\n";
  // clang-format on
  return str;
}

}  // namespace message
