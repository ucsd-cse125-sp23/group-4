#pragma once

#include <map>
#include <string>

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Player.h"
#include "client/graphics/Scene.h"

class HUD {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Violety Crumble.ttf");
  Scene* scene;
  Texture map;
  float offset;

  explicit HUD(Scene* scn) {
    scene = scn;
    map.init("assets/image/tagguys/frame_0_delay-0.08s.png");
    offset = 10;
  }

  void draw(GLFWwindow* window);

  void drawLeaderboard(GLFWwindow* window, float scale,
                       std::map<std::string, Timer> player_times);

  void drawMinimap();

  void gameOver();
};