#include "Player.h"

#include <cmath>
#include <glm/glm.hpp>

#include "Input.h"
#include "client/graphics/AssimpAnimation.h"

using glm::mat4x4;
using glm::vec3;
using glm::vec4;

using namespace client;  // NOLINT

void Player::animate(float dt) {
  // traverse animation bones (SLOW)
  if (pmodel) pmodel->update(dt);
}

void Player::update(float dt) {
  // interpolate between old to new state
  updateInterpolate(dt);

  if (fx_jump) fx_jump->update(dt);
  if (fx_land) fx_land->update(dt);
  if (fx_item) fx_item->update(dt);
  if (fx_tag) fx_tag->update(dt);
  if (fx_tagStatus) fx_tagStatus->update(dt);

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

  return {id, moveWorld.x, 0, moveWorld.z, jumping, azimuth};
}

void Player::updateFromState(message::Player p) {
  if (fx_tagStatus) fx_tagStatus->creationRate = state.is_tagged ? 5.0f : 0.0f;

  // animation
  if (pmodel) {
    if (p.posy < 97.0f) {
      pmodel->setAnimation(
          AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::FALL));
    } else {
      if (p.is_moving) {
        pmodel->setAnimation(
            AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::WALK));
      } else {
        pmodel->setAnimation(
            AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::IDLE));
      }
    }
  }

  GameThing::updateFromState(state);
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

// Event FX
void Player::eventJump() {
  if (fx_jump) fx_jump->Emit(3);  // spew particles!

  // SFX!
  if (sfx_jump) sfx_jump->play(transform.position);

  // animation
  if (pmodel) {
    pmodel->setAnimation(
        AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::JUMP));
  }
}

void Player::eventLand() {
  // if (fx_land) fx_land->Emit(5);
}

void Player::eventItem() {
  // if (fx_item) fx_item->Emit(10);

  // SFX! turning this off for now because item pick up is broken and spamed
  // if (sfx_item) sfx_item->play(transform.position);
}

void Player::eventTag() {
  // SFX!
  if (sfx_tag) sfx_tag->play(transform.position);

  // animation
  if (pmodel) {
    pmodel->setAnimation(
        AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::TAG));
  }
}

void Player::eventTagged() {
  if (fx_tag) fx_tag->Emit(15);

  // animation
  if (pmodel) {
    pmodel->setAnimation(
        AssimpAnimation::AC_TO_NAME.at(AssimpAnimation::PLAYER_AC::TRIP));
  }
}
