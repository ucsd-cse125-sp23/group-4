#include "Player.h"

using namespace glm;

void Player::update(float dt) {

    vec3 moveLocal = vec3(0);

    // read inputs
    if (Input::GetInputState(InputAction::MoveForward) != InputState::None) {
		moveLocal += vec3(0,0,-1);
	}
	if (Input::GetInputState(InputAction::MoveBack) != InputState::None) {
		moveLocal += vec3(0,0,1);
	}
    if (Input::GetInputState(InputAction::MoveRight) != InputState::None) {
		moveLocal += vec3(1,0,0);
	}
	if (Input::GetInputState(InputAction::MoveLeft) != InputState::None) {
		moveLocal += vec3(-1,0,0);
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
    // movement is in world space!

    // orient to face in direction
    //this->transform.rotation = 
    // -1.0 * camera->GetAzimuth()

    this->transform.position += movement;


    transform.updateMtx(&transformMtx);
}

