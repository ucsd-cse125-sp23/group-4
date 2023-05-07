#include "Player.h"

using glm::mat4x4;
using glm::vec3;

void client::Player::setPosition(vec3f pos) {
  GameThing::setPosition(vec3(pos.x, pos.y, pos.z));
  // GameThing::setPosition(static_cast<vec3>(pos));
}

void client::Player::update(float dt) {
  // core:
  if (coreRef_object) {
    setPosition(coreRef_object->getPos());
  }
  // --

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
  moveLocal *= speed * 0.015f;  // *dt;
  bool moving = length(moveLocal) > 0;

  if (moving) {
    move(moveLocal);
    pmodel->update(dt);
  } else {
    if (coreRef_control) {
      coreRef_control->horizontalVel = vec3f(0, 0, 0);
    }
  }

  if (tagged) time += dt;
}

void client::Player::OnEvent(const InputEvent evt) {
  // see InputListener

  if (evt.action == InputAction::MoveJump) {
    if (coreRef_control) {
      coreRef_control->doJump = false;  // always start false
    }
  }

  if (evt.state == InputState::Press) {
    if (evt.action == InputAction::MoveJump) {
      if (coreRef_control) {
        coreRef_control->doJump = true;
      }
    }
  }
}

void client::Player::move(vec3 movement) {
  // use camera data here
  if (camera) {
    movement = vec3(camera->getCameraRotationMtx() * vec4(-movement, 1));
  }

  faceDirection(movement);
  // GameThing::move(movement); // sp only!

  if (coreRef_control) {
    coreRef_control->horizontalVel = vec3f(movement.x, movement.y, movement.z);
  }
}

void client::Player::faceDirection(vec3 direction) {
  direction = normalize(direction);
  azimuth = std::atan2(direction.x, direction.z) + (M_PI);  // aka azimuth

  // purely visual, for now (rotation never applied to player node itself)
  if (!pmodel) return;

  pmodel->modelMtx = glm::eulerAngleY(azimuth);
}
