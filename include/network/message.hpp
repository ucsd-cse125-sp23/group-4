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

  template <typename Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& player_id;
    ar& time;
  }
};

struct GreetingBody {
  std::string greeting;

  template <typename Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& greeting;
  }
};

struct Movement {
  std::string direction;

  template <typename Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& direction;
  }
};

struct Message {
  Type type;
  Metadata metadata;
  GreetingBody body;
  // std::variant<struct Greeting, struct Movement> body;

  template <typename Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& type;
    ar& metadata;
    ar& body;
  }

  std::string to_string() const;
  friend std::ostream& operator<<(std::ostream&, const Message&);
};

}  // namespace message
