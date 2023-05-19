#pragma once

#include <map>
#include <network/message.hpp>

#include "glm/glm.hpp"

/* PROTOTYPE SCENE GAME STATE CODE (clean it up later) */

struct SceneGameThingState {
  int id;  // player id? fix this TODO
  glm::vec3 position;
  float heading;
  // add more here... onground, jump, animations, etc.

  SceneGameThingState() {}
  SceneGameThingState(message::GameStateUpdateItem me) {
    id = me.id;
    position = glm::vec3(me.posx, me.posy, me.posz);
    heading = me.heading;
  }
};

// received from server and needs to be drawn
struct SceneState {
  // want to be able to map an id to a GameThingState
  // try a different data structure?
  std::map<int, SceneGameThingState> objectStates;

  SceneGameThingState GetUpdateFor(int id) { return objectStates[id]; }

  SceneState() {}

  explicit SceneState(message::GameStateUpdate state) {
    // convert message to SceneState

    std::vector<message::GameStateUpdateItem*> thingStates;

    for (auto i : thingStates) {
      objectStates[i->id] = SceneGameThingState(*i);
    }
  }
};

/* *** */
