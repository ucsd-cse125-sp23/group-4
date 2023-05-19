#pragma once

#include <map>
#include <network/message.hpp>
#include <vector>

#include "glm/glm.hpp"

/* PROTOTYPE SCENE GAME STATE CODE (clean it up later) */

struct SceneGameThingState {
  int id;  // player id? fix this TODO
  glm::vec3 position;
  float heading;
  // add more here... onground, jump, animations, etc.

  SceneGameThingState() {}
  explicit SceneGameThingState(message::GameStateUpdateItem me) {
    id = me.id;
    position = glm::vec3(me.posx, me.posy, me.posz);
    heading = me.heading;
  }
};

// received from server and needs to be drawn
struct SceneState {
  // want to be able to map an id to a GameThingState
  std::map<int, SceneGameThingState> objectStates;

  SceneGameThingState GetUpdateFor(int id) { return objectStates[id]; }

  SceneState() {}

  // convert message to SceneState
  explicit SceneState(message::GameStateUpdate state) {
    // take list of all things and convert each one
    for (auto i : state.things) {
      objectStates[i->id] = SceneGameThingState(*i);
    }
  }
};

/* *** */
