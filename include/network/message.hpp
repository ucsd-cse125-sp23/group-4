#pragma once

#include <boost/serialization/variant.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/variant.hpp>
#include <ctime>
#include <ostream>
#include <string>

namespace message {

using PlayerID = boost::uuids::uuid;

enum class Type {
  Assign,
  Greeting,
  Notify,
};

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

struct Message {
  Type type;
  Metadata metadata;
  boost::variant<Assign, Greeting, Notify> body;

  std::string toString() const;
  friend std::ostream& operator<<(std::ostream&, const Message&);

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& type& metadata& body;
  }
};

}  // namespace message
