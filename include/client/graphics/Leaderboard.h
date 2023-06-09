#pragma once
#include <map>
#include <string>
#include <vector>

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Texture.h"

class Leaderboard {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Atma-SemiBold.ttf");
  Texture background;
  std::map<std::string, Texture> icons;
  std::map<int, Texture> name_tags;

  int flip = 1;
  int frames = 0;
  Leaderboard();

  ~Leaderboard() { delete fr; }

  void draw();

  void drawPlayers(std::vector<std::pair<int, std::string>> rankings);
};
