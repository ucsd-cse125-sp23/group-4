#pragma once

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Scene.h"

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