#pragma once

#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/variant.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/variant.hpp>
#include <ctime>
#include <ostream>
#include <string>
#include <unordered_map>

namespace message {

using ClientID = boost::uuids::uuid;

enum class Type { Assign, Greeting, Notify, GameStateUpdate, UserStateUpdate };

struct Metadata {
  ClientID id;       // client id
  std::time_t time;  // time of request being sent

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& id& time;
  }
};

struct Assign {
  int pid;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& pid;
  }
};

struct Greeting {
  std::string greeting;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& greeting;
  }
};

struct Notify {
  std::string message;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& message;
  }
};

struct GameStateUpdateItem {
  int id;
  float posx;
  float posy;
  float posz;
  float heading;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& id& posx& posy& posz& heading;
  }
};

struct GameStateUpdate {
  std::unordered_map<int, GameStateUpdateItem> things;
  // add global params later

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& things;
  }
};

struct UserStateUpdate {
  int id = -1;  // assume -1 IDs invalid
  float movx;
  float movy;
  float movz;
  bool jump;
  float heading;

  std::string to_string() const;

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& players;
  }
};

struct Message {
  using Body = boost::variant<Assign, Greeting, Notify, GameStateUpdate,
                              UserStateUpdate>;
  Type type;
  Metadata metadata;
  Body body;
  std::string to_string() const;
  friend std::ostream& operator<<(std::ostream&, const Message&);

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& type& metadata& body;
  }
};

Type get_type(const Message::Body&);

}  // namespace message
