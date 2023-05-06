#include "Player.h"

using glm::mat4x4;
using glm::vec3;

void Player::update(float dt) {
  if (camera && camera->Fixed)
    return;  // don't move the player in no clip (for now)

  vec3 moveLocal = vec3(0);

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

  if (moving) {
    move(moveLocal);
    pmodel->update(dt);
  }

  if (tagged) time += dt;
}

void Player::move(vec3 movement) {
  // use camera data here
  if (camera) {
    movement = vec3(camera->getCameraRotationMtx() * vec4(-movement, 1));
  }

  faceDirection(movement);
  GameThing::move(movement);
}

void Player::faceDirection(vec3 direction) {
  direction = normalize(direction);
  azimuth = std::atan2(direction.x, direction.z) + (M_PI);  // aka azimuth

  // purely visual, for now (rotation never applied to player node itself)
  if (!pmodel) return;

  pmodel->modelMtx = glm::eulerAngleY(azimuth);
}
