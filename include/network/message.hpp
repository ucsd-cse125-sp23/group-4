#pragma once

#include <boost/serialization/variant.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/variant.hpp>
#include <ctime>
#include <ostream>
#include <string>
#include <vector>

namespace message {

using PlayerID = boost::uuids::uuid;

enum class Type { Assign, Greeting, Notify, GameStateUpdate, UserStateUpdate };

struct Metadata {
  PlayerID player_id;
  std::time_t time;

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& player_id& time;
  }
};

struct Assign {
  std::string toString() { return "assign response"; }

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {}
};

struct Greeting {
  std::string greeting;
  std::string toString() { return greeting; }

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& greeting;
  }
};

struct Notify {
  std::string message;
  std::string toString() { return message; }

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& message;
  }
};

struct GameStateUpdateItem {
  friend class boost::serialization::access;
  int id;
  float posx;
  float posy;
  float posz;
  float heading;

  GameStateUpdateItem() {}

  std::string toString() {
    // clang-format off
    std::string str = std::string("") + "{\n" +
                      "  id: " + std::to_string(id) +
                      ",\n" +  // NOLINT
                      "  position: {\n" +
                      "  " + std::to_string(posx) +
                      ", " + std::to_string(posy) +
                      ", " + std::to_string(posz) +
                      "\n" +  // NOLINT
                      "  },\n" +
                      "  heading: " + std::to_string(heading) +
                      ",\n" +  // NOLINT
                      "}";
    // clang-format on
    return str;
  }

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& id;
    ar& posx;
    ar& posy;
    ar& posz;
    ar& heading;
  }
};

struct GameStateUpdate {
  std::vector<GameStateUpdateItem*> things;
  // add global params later

  std::string toString() {
    // clang-format off
    std::string str = std::string("") + "{\n" +
                      "items:\n";
    // clang-format on
    for (auto i : things) {
      str += i->toString();
    }
    return str;
  }

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& things;
  }
};
struct UserStateUpdate {
  int id;
  float movx;
  float movy;
  float movz;
  bool jump;
  float heading;

  std::string toString() {
    // clang-format off
    std::string str = std::string("") + "{\n" +
                      "  id: " + std::to_string(id) +
                      ",\n" +  // NOLINT
                      "  movement delta: {\n" +
                      "  " + std::to_string(movx) +
                      ", " + std::to_string(movy) +
                      ", " + std::to_string(movz) +
                      "\n" +  // NOLINT
                      "  },\n" +
                      "  jump: " + std::to_string(jump) +
                      ",\n" +  // NOLINT
                      "  heading: " + std::to_string(heading) +
                      ",\n" +  // NOLINT
                      "}";
    // clang-format on
    return str;
  }

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& id;
    ar& movx;
    ar& movy;
    ar& movz;
    ar& jump;
    ar& heading;
  }
};

struct Message {
  Type type;
  Metadata metadata;
  boost::variant<Assign, Greeting, Notify, GameStateUpdate, UserStateUpdate>
      body;

  std::string toString() const;
  friend std::ostream& operator<<(std::ostream&, const Message&);

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& type& metadata& body;
  }
};

}  // namespace message
