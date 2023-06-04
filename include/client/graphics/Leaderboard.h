#pragma once
#include <string>
#include <vector>

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Input.h"
#include "client/graphics/InputListener.h"
#include "client/graphics/Scene.h"

class Leaderboard : public Scene, public InputListener {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Atma-SemiBold.ttf");
  Texture background;

  explicit Leaderboard(Camera* camFromWindow);

  void init(void) override;
  void update(float delta) override;
  void draw() override;

};
