#pragma once
#include "client/graphics/Scene.h"
#include "client/graphics/InputListener.h"
#include "client/graphics/Input.h"
#include "client/graphics/FontRenderer.h"

class Start : public Scene, public InputListener {
 public:
  FontRenderer fr;
  Texture background;
  float timeElapsed;
  float offset;
  bool gameStart;
  bool renderText;

  Start(Camera* camFromWindow) : Scene(camFromWindow) {
	gamethings.clear();
    timeElapsed = 0.0f;
	offset = 0.0f;
	background.init("assets/image/clouds.png");
    gameStart = false;
	renderText = true;
  }

  void draw();

  void drawBackground();

  void update(float delta);
};