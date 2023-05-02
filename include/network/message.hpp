#pragma once

#include <ctime>
#include <ostream>
#include <string>
#include <variant>

namespace message {
enum Type {
  Greeting,
  Join,
  Customize,
  Ready,
  Movement,
  Item,
  GameState,
  Animation
};

struct Metadata {
  int player_id;
  std::time_t time;
};

struct Greeting {
  std::string greeting;

  template <typename Archive>
  void serialize(Archive& ar) {
    ar << greeting;
  }
};

struct Movement {
  std::string direction;

  template <typename Archive>
  void serialize(Archive& ar) {
    ar << direction;
  }
};

struct Message {
  Type type;
  Metadata metadata;
  // std::variant<struct Greeting, struct Movement> body;
  struct Greeting body;

  template <typename Archive>
  void serialize(Archive& ar) {
    ar << type << metadata << body;
  }

  friend std::ostream& operator<<(std::ostream& os, const Message& dt);
};

}  // namespace message
