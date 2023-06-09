#pragma once
#include <map>
#include <string>
#include <vector>
#include <map>

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Texture.h"

class Leaderboard {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Atma-SemiBold.ttf");
  Texture background;
  std::map<std::string, Texture> icons;
  int flip = 1;
  int frames = 0;
  Leaderboard();

  ~Leaderboard() { delete fr; }

  void draw();

  void drawPlayers(std::vector<std::string> rankings);
};
