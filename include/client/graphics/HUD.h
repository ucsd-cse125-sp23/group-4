#pragma once

#include "FontRenderer.h"
#include "Scene.h"
#include "Player.h"

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