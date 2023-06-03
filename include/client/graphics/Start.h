#pragma once
#include <string>
#include <vector>

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Input.h"
#include "client/graphics/InputListener.h"
#include "client/graphics/Scene.h"

class Start : public Scene, public InputListener {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Violety Crumble.ttf");
  Texture background;
  std::vector<Texture> frames;
  float timeOnFrame;
  int index;
  float timeElapsed;
  float offset;
  bool joinGame;
  bool renderText;

  explicit Start(Camera* camFromWindow) : Scene(camFromWindow) {
    localGameThings.clear();
    timeElapsed = 0.0f;
    offset = 0.0f;
    background.init("assets/image/clouds.png");
    joinGame = false;
    renderText = true;

    timeOnFrame = 0;
    index = 0;
    for (int i = 1; i < 33; i++) {
      Texture frame;
      std::string filename = "assets/image/tagguys_open/frame_" +
                             std::to_string(i) + "_delay-0.1s.png";
      frame.init(filename.c_str());
      frames.push_back(frame);
    }
  }

  void draw();

  void drawBackground();

  void drawName();

  void update(float delta, GamePhase& phase, bool& transition);
};
