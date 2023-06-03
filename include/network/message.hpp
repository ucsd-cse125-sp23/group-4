#pragma once

#include <boost/serialization/string.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/variant.hpp>
#include <boost/serialization/vector.hpp>
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

enum class Type {
  Assign,
  Greeting,
  Notify,
  GameStateUpdate,
  UserStateUpdate,
  LobbyUpdate,
  LobbyPlayerUpdate,
  GameStart,
};

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

  long score;
  float speed;

  bool is_grounded;
  bool is_tagged;

  std::vector<std::string> has_powerup;

  bool has_jumped;
  bool has_landed;
  bool has_obtained;
  bool has_tagged;
  bool has_been_tagged;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& id& posx& posy& posz& heading& score& speed& is_grounded& is_tagged&
        has_powerup& has_jumped& has_landed& has_obtained& has_tagged&
            has_been_tagged;
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
    ar& id& movx& movy& movz& jump& heading;
  }
};

struct LobbyPlayer {
  int id;
  std::string skin;
  bool is_ready;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& id& skin& is_ready;
  }
};
using LobbyPlayerUpdate = LobbyPlayer;  // type alias for client usage

struct LobbyUpdate {
  std::unordered_map<int, LobbyPlayer> players;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& players;
  }
};

struct GameStart {
  std::unordered_map<int, bool> client_states;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& client_states;
  }
};

struct Message {
  using Body =
      boost::variant<Assign, Greeting, Notify, GameStateUpdate, UserStateUpdate,
                     LobbyUpdate, LobbyPlayerUpdate, GameStart>;
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
