#pragma once

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include "client/graphics/GameThing.h"
#include "client/graphics/PlayerModel.h"
#include "client/graphics/Skeleton.h"
#include "client/graphics/SkinnedMesh.h"
#include "client/graphics/AnimationPlayer.h"
#include "client/graphics/Camera.h"
#include "client/graphics/Input.h"
#include "client/graphics/InputListener.h"

#include <map>
#include <string>

class Player : public GameThing, InputListener {
  public:
    float speed = 10;

    Camera* camera; // TODO

    PlayerModel* pmodel;
    float time;
    bool tagged;

    Player() {
        time = 0;
        pmodel = nullptr;
        tagged = true;
    }

    void update(float dt);

    void move(vec3 movement);
};