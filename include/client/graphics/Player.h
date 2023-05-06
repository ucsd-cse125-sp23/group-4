/**************************************************
 * Player.h
 * a single local player, controlled by the user
 * stores information about visuals in PlayerModel.h
 * currently needs Node::model reference set manually
 *****************************************************/

#pragma once

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <map>
#include <string>

#include "client/graphics/AnimationPlayer.h"
#include "client/graphics/Camera.h"
#include "client/graphics/GameThing.h"
#include "client/graphics/Input.h"
#include "client/graphics/InputListener.h"
#include "client/graphics/PlayerModel.h"
#include "client/graphics/Skeleton.h"
#include "client/graphics/SkinnedMesh.h"
#include "core/game/modifier/ControlModifier.h"
#include "core/math/vector.h"

namespace client {

class Player : public GameThing, public InputListener {
 private:
  float azimuth = 0;

 public:
  PObject* coreRef_object;  // this will be used to show position, for now
  ControlModifierData*
      coreRef_control;  // this will be used to show physics, for now

  float speed = 10;

  Camera* camera =
      nullptr;  // a reference to the scene camera, so we know where we're going

  PlayerModel* pmodel;  // visual information

  float time;  // how long we've been tagged
  bool tagged;

  Player() {
    time = 0;
    pmodel = nullptr;
    tagged = true;
  }

  void OnEvent(const InputEvent evt);

  void update(float dt);
  void move(vec3 movement);

  void faceDirection(vec3 direction);

  // networking interface v00001  --
  void setPosition(vec3f pos);
  void setHeading(float rot);
  //                              --

  string debug_info() {
    std::ostringstream str;
    str << coreRef_object->getPos();
    string out = "player" + to_string(coreRef_object->id) + "\n";
    out += "player pos: " + str.str() + "\n";

    std::ostringstream str2;
    str2 << coreRef_object->vel;
    out += ("player vel: " + str2.str()) + "\n";

    out += "player doJump: " + to_string(coreRef_control->doJump);
    return out;
  }
};

}  // namespace client