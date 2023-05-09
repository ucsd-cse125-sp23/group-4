#pragma once

#include <boost/serialization/variant.hpp>
#include <boost/variant.hpp>
#include <ctime>
#include <ostream>
#include <string>

namespace message {

enum class Type {
  Connect,
  Greeting,
};

struct Metadata {
  int player_id;
  std::time_t time;

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& player_id& time;
  }
};

struct Connect {
  std::string toString() { return "connect request"; }

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

struct Message {
  Type type;
  Metadata metadata;
  boost::variant<Connect, Greeting> body;

  std::string toString() const;
  friend std::ostream& operator<<(std::ostream&, const Message&);

  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& type& metadata& body;
  }
};

}  // namespace message
