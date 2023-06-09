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

    for (int i = 0; i < 40; i++) {
      Texture frame;
      std::string filename = "assets/image/countdown/frame_" +
                             std::to_string(i) + "_delay-0.1s.png";
      frame.init(filename.c_str());
      frames.push_back(frame);
    }

    timer.init("assets/UI/IMG_2658.PNG");

    player_bars["trash panda"].init("assets/UI/bar-racoon.png");
    player_bars["bee"].init("assets/UI/bar-bee.png");
    player_bars["avocado"].init("assets/UI/bar-avocado.png");
    player_bars["duck"].init("assets/UI/bar-duck.png");
    player_bars["cat"].init("assets/UI/bar-cat.png");
    player_bars["unicorn"].init("assets/UI/bar-unicorn.png");
    player_bars["waffle"].init("assets/UI/bar-waffle.png");
    player_bars["bear"].init("assets/UI/Group 6 (1).png");
  }

  void draw(GLFWwindow* window);

  void drawTime();

  void drawLeaderboard(GLFWwindow* window, float scale,
                       std::map<int, Player*> players);

  void drawBar(std::string skin);

  void drawMinimap();

  void update();

  void drawCountdown();

  void gameOver();

  void reset();
};
