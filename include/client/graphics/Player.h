/**************************************************
 * Player.h
 * a single local player, controlled by the user
 * stores information about visuals in PlayerModel.h
 * currently needs Node::model reference set manually
 *****************************************************/

#pragma once

#include <utility>

#include "Camera.h"
#include "GameThing.h"
#include "InputListener.h"
#include "PlayerModel.h"

class Player : public GameThing, InputListener {
 public:
  float speed = 10;

  Camera* camera =
      nullptr;  // a reference to the scene camera, so we know where we're going

  PlayerModel* pmodel;  // visual information

  Player() {
    pmodel = nullptr;
  }

  message::UserStateUpdate pollInput();

  void update(float dt);
  glm::vec3 move(glm::vec3 movement);

  void faceDirection(glm::vec3 direction);
};
