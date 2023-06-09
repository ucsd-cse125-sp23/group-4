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
#include "Input.h"
#include "InputListener.h"
#include "ParticleSystem.h"
#include "PlayerModel.h"
#include "Timer.h"

namespace client {

class Player : public GameThing, InputListener {
 public:
  float speed = 10;

  Camera* camera =
      nullptr;  // a reference to the scene camera, so we know where we're going

  PlayerModel* pmodel;  // visual information

  ParticleSystem* fx_jump;
  ParticleSystem* fx_land;
  ParticleSystem* fx_item;
  ParticleSystem* fx_tag;
  ParticleSystem* fx_tagStatus;
  SoundEffect* sfx_jump;
  SoundEffect* sfx_item;
  SoundEffect* sfx_tag;
  int score;  // number of ticks not it
  bool tagged;

  Player() {
    pmodel = nullptr;
    tagged = true;
    sfx_jump = nullptr;
    sfx_item = nullptr;
    sfx_tag = nullptr;
    fx_jump = nullptr;
    fx_land = nullptr;
    fx_item = nullptr;
    fx_tag = nullptr;
    fx_tagStatus = nullptr;
  }

  ~Player() {
    if (fx_jump) delete fx_jump;
    if (fx_land) delete fx_land;
    if (fx_item) delete fx_item;
    if (fx_tag) delete fx_tag;
    if (sfx_jump) delete sfx_jump;
    if (sfx_tag) delete sfx_tag;
    if (sfx_item) delete sfx_item;
  }

  message::UserStateUpdate pollInput();
  void updateFromState(message::Player p) override;

  void eventJump();
  void eventLand();
  void eventItem();
  void eventTag();
  void eventTagged();

  void animate(float dt) override;
  void update(float dt);
  glm::vec3 move(glm::vec3 movement);

  void faceDirection(glm::vec3 direction);
};

}  // namespace client
