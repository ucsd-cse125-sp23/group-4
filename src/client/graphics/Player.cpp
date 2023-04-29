#include "Player.h"

using namespace glm;

void Player::update(float dt) {

    vec3 moveLocal = vec3(0);

    // read inputs
    if (Input::GetInputState(InputAction::MoveForward) != InputState::None) {
		moveLocal += vec3(0,0,1);
	}
	if (Input::GetInputState(InputAction::MoveBack) != InputState::None) {
		moveLocal += vec3(0,0,-1);
	}
    if (Input::GetInputState(InputAction::MoveRight) != InputState::None) {
		moveLocal += vec3(-1,0,0);
	}
	if (Input::GetInputState(InputAction::MoveLeft) != InputState::None) {
		moveLocal += vec3(1,0,0);
	}

    moveLocal = normalize(moveLocal);

    moveLocal *= speed * dt;

    if (length(moveLocal) > 0) move(moveLocal);


  Skeleton* skel = pmodel->skel;
  SkinnedMesh* skin = pmodel->skin;
  std::map<std::string, AnimationPlayer*> anims = pmodel->anims;
  skel->Update(dt, anims);
  skin->updateSkin(skel);
  if (tagged) time += dt;
}

void Player::move(vec3 movement) {
	// use camera data here
	if (camera) {
        if (camera->Fixed) return;

        movement = vec3(camera->getCameraRotationMtx() * vec4(-movement, 1));
	}

    GameThing::move(movement);
}

