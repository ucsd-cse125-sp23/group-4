#pragma once

#include "FontRenderer.h"
#include "Player.h"
#include "Scene.h"

class HUD {
 public:
  FontRenderer fr;
  Scene* scene;
  Texture map;

  HUD(Scene* scn) {
    scene = scn;
    map.init("assets/image/test_uv.png");
  }

  void draw(GLFWwindow* window);

  void drawMinimap();
};