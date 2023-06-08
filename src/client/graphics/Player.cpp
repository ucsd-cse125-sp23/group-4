#include "Player.h"

#include <cmath>
#include <glm/glm.hpp>

#include "Input.h"
#include "client/graphics/AssimpAnimation.h"

using glm::mat4x4;
using glm::vec3;
using glm::vec4;

void Player::animate(float dt) {
  // traverse animation bones (SLOW)
  if (pmodel) pmodel->update(dt);
}

void Player::update(float dt) {
  // interpolate between old to new state
  updateInterpolate(dt);

  if (fx_jump) fx_jump->update(dt);

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
    if (fx_jump) fx_jump->Emit(5);  // TODO(matthew) move this to jump event msg

    jumping = true;
  }

  moveLocal = normalize(moveLocal);
  // moveLocal *= speed * dt;  // this is for single-player
  bool moving = length(moveLocal) > 0;

  vec3 moveWorld = vec3(0);

  if (moving) {
    moveWorld = move(moveLocal);
  } else {
  }

  if (jumping) {
    if (pmodel) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::JUMP));
    }
  }

  if (pmodel) {
    if (Input::GetInputState(InputAction::WALK) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::WALK));
    }
    if (Input::GetInputState(InputAction::IDLE) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::IDLE));
    }
    if (Input::GetInputState(InputAction::TRIP) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::TRIP));
    }
    if (Input::GetInputState(InputAction::FALL) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::FALL));
    }
    if (Input::GetInputState(InputAction::WINNING11) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::PLACE1_1));
    }
    if (Input::GetInputState(InputAction::WINNING12) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::PLACE1_2));
    }
    if (Input::GetInputState(InputAction::WINNING21) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::PLACE2_1));
    }
    if (Input::GetInputState(InputAction::WINNING22) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::PLACE2_2));
    }
    if (Input::GetInputState(InputAction::WINNING31) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::PLACE3_1));
    }
    if (Input::GetInputState(InputAction::WINNING32) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::PLACE3_2));
    }
    if (Input::GetInputState(InputAction::WINNING41) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::PLACE4_1));
    }
    if (Input::GetInputState(InputAction::WINNING42) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::PLACE4_2));
    }
    if (Input::GetInputState(InputAction::LOBBY1) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::LOBBY1));
    }
    if (Input::GetInputState(InputAction::LOBBY2) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::LOBBY2));
    }
    if (Input::GetInputState(InputAction::LOBBY3) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::LOBBY3));
    }
    if (Input::GetInputState(InputAction::LOBBY4) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::LOBBY4));
    }
    if (Input::GetInputState(InputAction::TAG) != InputState::None) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::TAG));
    }
  }

  return {id, moveWorld.x, 0, moveWorld.z, jumping, azimuth};
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
