#pragma once

#include "FontRenderer.h"
#include "Player.h"
#include "Scene.h"

class HUD {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Violety Crumble.ttf");
  Scene* scene;
  Texture map;
  float offset;

  HUD(Scene* scn) {
    scene = scn;
    map.init("assets/image/sample.png");
    offset = 10;
  }

  void draw(GLFWwindow* window);

  void drawLeaderboard(GLFWwindow* window, float scale, std::map<std::string, Timer> player_times);

  void drawMinimap();

  void gameOver();
};