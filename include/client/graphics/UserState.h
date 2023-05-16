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

  message::Message toMessage() {
    // TODO(matthew) convert state to message to be sent
  }
};

/* *** */
