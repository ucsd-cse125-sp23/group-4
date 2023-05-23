#include "Player.h"

#include <cmath>
#include <glm/glm.hpp>

#include "Input.h"
#include "UserState.h"

using glm::mat4x4;
using glm::vec3;
using glm::vec4;

UserState Player::update(float dt) {
  if (!isUser) return UserState();

  if (camera && camera->Fixed)
    return UserState();  // don't move the player in no clip (for now)

  vec3 moveLocal = vec3(0);  // relative to... keyboard

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

  moveLocal = normalize(moveLocal);
  moveLocal *= speed * dt;
  bool moving = length(moveLocal) > 0;

  vec3 moveWorld = vec3(0);
  if (moving) {
    moveWorld = move(moveLocal);
    if (pmodel) pmodel->update(dt);
  }

  if (tagged) time += dt;  // move this to server TODO

  // Get ready to send a message to the server: ***
  UserState myInputState;
  myInputState.id = netId;
  myInputState.movement = moveWorld;
  myInputState.heading = azimuth;
  myInputState.jump = false;

  return myInputState;
}

vec3 Player::move(vec3 movement) {
  // use camera data here
  if (camera) {
    movement = vec3(camera->getCameraRotationMtx() * vec4(-movement, 1));
  }

  faceDirection(movement);
  // GameThing::move(movement);  // don't actually move. let the server do it

  return movement;  // send back the "world space" movement vector
}

void Player::faceDirection(vec3 direction) {
  direction = normalize(direction);
  azimuth = std::atan2(direction.x, direction.z) + (M_PI);  // aka azimuth

  // purely visual, for now (rotation never applied to player node itself)
  if (!model) return;

  model->modelMtx = glm::eulerAngleY(azimuth);
}
