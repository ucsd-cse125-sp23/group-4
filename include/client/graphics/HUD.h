#pragma once

#include <map>
#include <string>
#include <vector>

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Player.h"
#include "client/graphics/Scene.h"

class HUD {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Violety Crumble.ttf");
  Scene* scene;
  Texture map;
  std::vector<Texture> frames;
  std::map<std::string, Texture> icons;
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
      std::string filename = "assets/image/countdown/frame_" +
                             std::to_string(i) + "_delay-0.1s.png";
      frame.init(filename.c_str());
      frames.push_back(frame);
    }

    icons["neutral"].init("assets/icons/neutral.png");
    icons["angry"].init("assets/icons/angry.png");
    icons["blushing"].init("assets/icons/blushing.png");
    icons["crying"].init("assets/icons/crying.png");
    icons["heart"].init("assets/icons/heart.png");
    icons["side_eye"].init("assets/icons/side_eye.png");
  }

  void draw(GLFWwindow* window);

  void drawLeaderboard(GLFWwindow* window, float scale,
                       std::map<std::string, Player*> players);

  void drawIcon(std::string icon);

  void drawMinimap();

  void update();

  void drawCountdown();

  void gameOver();
};
