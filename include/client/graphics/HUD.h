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
    map.init("assets/image/sample.png");
  }

  void draw(GLFWwindow* window);

  void drawLeaderboard(GLFWwindow* window, float scale, std::map<std::string, Timer> player_times);

  void drawMinimap();
};