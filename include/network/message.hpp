#pragma once

#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/variant.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/variant.hpp>
#include <ctime>
#include <network/effect.hpp>
#include <network/item.hpp>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/game/event/Event.h"

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
  JumpEvent,
  LandEvent,
  ItemPickupEvent,
  TagEvent,
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

struct Player {
  int id;
  float posx;
  float posy;
  float posz;
  float heading;
  int score;
  float speed;
  int ticks_fallen;
  int is_grounded;
  bool is_tagged;
  std::vector<Effect> effects;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& id& posx& posy& posz& heading& score& speed& ticks_fallen& is_grounded&
        is_tagged& effects;
  }
};

struct Item {
  int id;
  ::Item item;
  float posx;
  float posy;
  float posz;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& id& item& posx& posy& posz;
  }
};

struct GameStateUpdate {
  std::unordered_map<int, Player> players;
  std::unordered_map<int, Item> items;
  int tagged_player;
  float time_elapsed;
  float time_remaining;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& players& items& tagged_player& time_elapsed& time_remaining;
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

struct JumpEvent {
  int pid;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& pid;
  }
};

struct LandEvent {
  int pid;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& pid;
  }
};

struct ItemPickupEvent {
  int pid;
  ::Item item;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& pid& item;
  }
};

struct TagEvent {
  int tagger;
  int taggee;

  std::string to_string() const;
  template <typename Archive>
  void serialize(Archive& ar, unsigned int) {
    ar& tagger& taggee;
  }
};

struct Message {
  using Body =
      boost::variant<Assign, Greeting, Notify, GameStateUpdate, UserStateUpdate,
                     LobbyUpdate, LobbyPlayerUpdate, GameStart, JumpEvent,
                     LandEvent, ItemPickupEvent, TagEvent>;
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
