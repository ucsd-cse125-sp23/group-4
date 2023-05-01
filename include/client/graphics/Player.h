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
  private:
    float azimuth = 0;
  public:
    float speed = 10;

    Camera* camera = nullptr;   // a reference to the scene camera, so we know where we're going

    PlayerModel* pmodel;    // visual information

    float time; // how long we've been tagged
    bool tagged;

    Player() {
        time = 0;
        pmodel = nullptr;
        tagged = true;
    }

    void update(float dt);
    void move(vec3 movement);

    void faceDirection(vec3 direction);

    // networking interface v00001  --
    void setPosition(vec3 pos);
    void setHeading(float rot);
    //                              --
};