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
    ar& id;
    ar& posx;
    ar& posy;
    ar& posz;
    ar& heading;
  }
};

struct GameStateUpdate {
  std::vector<GameStateUpdateItem> things;
  // add global params later
  std::string to_string() const;

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
  std::string to_string() const;

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
