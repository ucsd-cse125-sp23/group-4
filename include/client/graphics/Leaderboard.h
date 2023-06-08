#pragma once
#include <string>
#include <vector>

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Texture.h"

class Leaderboard {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Atma-SemiBold.ttf");
  Texture background;

  Leaderboard();

  ~Leaderboard() { delete fr; }

  void draw();
};
