#pragma once

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "GameThing.h"
#include "PlayerModel.h"
#include "Skeleton.h"
#include "SkinnedMesh.h"
#include "AnimationPlayer.h"
#include "Camera.h"
#include <map>
#include <string>

class Player : public GameThing {
 public:
  PlayerModel* mod;
  void update(GLFWwindow* window, Camera* camera, float dt, float step = 0.25) {
    Skeleton* skel = mod->skel;
    SkinnedMesh* skin = mod->skin;
    std::map<std::string, AnimationPlayer*> anims = mod->anims;
    skel->Update(window, camera, dt, anims, step);
    skin->updateSkin(skel);
  }
};