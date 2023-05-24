#pragma once

#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include <core/lib.hpp>
#include <network/message.hpp>
#include <unordered_map>
#include <vector>

#include "client/graphics/ColliderImporter.h"

struct GameThing {
  int id;
  float heading;
  Player* player;
  ControlModifierData* control;

  inline GameThing(int& id, Player* p, ControlModifierData* c)
      : id(id), player(p), control(c), heading(0) {}

  inline void move(float x, float y, float z) {  // NOLINT
    control->horizontalVel = vec3f(x, y, z);
  }

  inline void update(const message::UserStateUpdate& update) {
    std::cout << "(GameThing::update) Updating GameThing " << update.id
              << std::endl;
    control->horizontalVel = vec3f(update.movx, update.movy, update.movz);
    control->doJump = update.jump;
    heading = update.heading;
  }

  message::GameStateUpdateItem to_network() {
    return {id, player->getPos().x, player->getPos().y, player->getPos().z,
            heading};
  }
};

using GameThingMap = std::unordered_map<int, GameThing>;

// stored in server
class Game {
 public:
  inline Game() {
    Environment* environment = new Environment();
    std::vector<ColliderData> mapColliders =
        ColliderImporter::ImportCollisionData(
            "assets/models/test_colliders.obj");
    for (auto collider : mapColliders) {
      environment->addConvex(collider.vertices, 0.2f);
    }
    initializeLevel(environment);
  }

  inline int create_player() {
    auto player_pair = initializePlayer();
    int new_id = next_pid++;
    game_things_.insert(
        {new_id, GameThing(new_id, player_pair.first, player_pair.second)});
    return new_id;
  }

  inline void update(const message::UserStateUpdate& update) {
    // TODO: remove this when proper assignment is implemented
    game_things_.at(update.id).update(update);
  }

  inline void tick() { level->tick(); }

  inline std::vector<message::GameStateUpdateItem> to_network() {
    std::vector<message::GameStateUpdateItem> things(game_things_.size());
    std::transform(game_things_.begin(), game_things_.end(), things.begin(),
                   [](auto&& thing) { return thing.second.to_network(); });
    return things;
  }

 private:
  GameThingMap game_things_;
  int next_pid = 1;
};
