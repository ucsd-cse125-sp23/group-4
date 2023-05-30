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
  std::vector<Texture> frames;
  int index;
  float offset;
  float timeOnFrame;
  double lastTime;

  explicit HUD(Scene* scn) {
    scene = scn;
    map.init("assets/image/tagguys/frame_24_delay-0.08s.png");
    offset = 10;
    index = 0;
    lastTime = glfwGetTime();
    index = 0;
    timeOnFrame = 0;

    for (int i = 0; i < 40; i++) {
      Texture frame;
      std::string filename = "assets/image/countdown/frame_" + std::to_string(i) +
                             "_delay-0.1s.png";
      frame.init(filename.c_str());
      frames.push_back(frame);
    }
  }

  void draw(GLFWwindow* window);

  void drawLeaderboard(GLFWwindow* window, float scale,
                       std::map<std::string, Player*> players);

  void drawMinimap();

  void update();

  void drawCountdown();

  void gameOver();
};
