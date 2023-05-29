#include "Player.h"

#include <cmath>
#include <glm/glm.hpp>

#include "Input.h"

using glm::mat4x4;
using glm::vec3;
using glm::vec4;

void Player::update(float dt) {
  // interpolate between old to new state
  updateInterpolate(dt);

  if (pmodel) pmodel->update(dt);

  if (tagged) time.Update(dt);
}

message::UserStateUpdate Player::pollInput() {
  if (!isUser) return message::UserStateUpdate();

  if (camera && camera->Fixed)
    return message::UserStateUpdate();  // don't move the player in no clip

  vec3 moveLocal = vec3(0);  // relative to... keyboard
  bool jumping = false;

  // read inputs
  if (Input::GetInputState(InputAction::MoveForward) != InputState::None) {
    moveLocal += vec3(0, 0, 1);
  }
  if (Input::GetInputState(InputAction::MoveBack) != InputState::None) {
    moveLocal += vec3(0, 0, -1);
  }
  if (Input::GetInputState(InputAction::MoveRight) != InputState::None) {
    moveLocal += vec3(-1, 0, 0);
  }
  if (Input::GetInputState(InputAction::MoveLeft) != InputState::None) {
    moveLocal += vec3(1, 0, 0);
  }

  if (Input::GetInputState(InputAction::MoveJump) != InputState::None) {
    jumping = true;
  }

  moveLocal = normalize(moveLocal);
  // moveLocal *= speed * dt;  // this is for single-player
  bool moving = length(moveLocal) > 0;

  vec3 moveWorld = vec3(0);
  if (moving) {
    moveWorld = move(moveLocal);
  }

  // Get ready to send a message to the server: ***
  message::UserStateUpdate myInputState;
  myInputState.id = netId;
  myInputState.movx = moveWorld.x;
  myInputState.movy = 0;
  myInputState.movz = moveWorld.z;
  myInputState.heading = azimuth;
  myInputState.jump = jumping;

  return myInputState;
}

vec3 Player::move(vec3 movement) {
  // use camera-relative movement
  if (camera) {
    movement = vec3(camera->getCameraRotationMtx() * vec4(-movement, 1));
  }

  setHeading(movement);
  // GameThing::move(movement);  // don't actually move. let the server do it

  return movement;  // send back the "world space" movement vector
}
