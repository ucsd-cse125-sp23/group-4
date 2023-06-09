#pragma once

#include <map>
#include <string>
#include <vector>

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Player.h"
#include "client/graphics/Scene.h"

class HUD {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Atma-SemiBold.ttf");
  FontRenderer* fr_dev = new FontRenderer("assets/fonts/Roboto-Black.ttf");

  Scene* scene;
  Texture map;
  Texture timer;
  std::vector<Texture> frames;
  std::map<std::string, Texture> player_bars;
  int index;
  float offset;
  float timeOnFrame;
  double lastTime;

  explicit HUD(Scene* scn) {
    scene = scn;
    map.init("assets/UI/IMG_2468.PNG");
    offset = 10;
    index = 0;
    lastTime = glfwGetTime();
    index = 0;
    timeOnFrame = 0;
  }

  void init();

  void draw(GLFWwindow* window);

  void drawTime();

  void drawLeaderboard(GLFWwindow* window, float scale,
                       std::map<std::string, Player*> players);

  void drawBar(std::string skin);

  void drawMinimap();

  void update();

  void drawCountdown();

  void gameOver();

  void reset();
};
