#pragma once

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Scene.h"

class HUD {
 public:
  FontRenderer fr;
  Scene* scene;

  HUD(Scene* scn) { scene = scn; }

  void draw(GLFWwindow* window);
};