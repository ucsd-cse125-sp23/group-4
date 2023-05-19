#pragma once

#include <network/message.hpp>

#include "glm/glm.hpp"

/* PROTOTYPE PLAYER INPUT STATE CODE (client -> server) */

struct UserState {
  int id;  // player id? fix this TODO
  glm::vec3 movement;
  bool jump;
  float heading;
  // add more here... onground, animations, etc.

  message::UserStateUpdate toMessage() {
    // convert state to message to be sent

    message::UserStateUpdate m = message::UserStateUpdate();

    m.id = id;
    m.movx = movement.x;
    m.movy = 0;
    m.movz = movement.z;

    return m;
  }
};

/* *** */
