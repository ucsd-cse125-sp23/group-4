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
  std::vector<Texture> frames;
  float timeOnFrame;
  int index;
  float timeElapsed;
  float offset;
  bool renderText;

  explicit Start(Camera* camFromWindow) : Scene(camFromWindow) {
    timeElapsed = 0.0f;
    offset = 0.0f;
    renderText = true;
    camera->SetDistance(600.0);
    camera->SetIncline(32.0);
    camera->SetPositionTarget(glm::vec3(124.0, 116.0, 6.0));
    timeOnFrame = 0;
    index = 0;
  }

  void init(void) override;

  void draw() override;

  void drawName();

  void update(float delta) override;
};
