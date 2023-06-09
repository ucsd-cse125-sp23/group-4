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
  Texture main;
  Texture credits;
  float timeOnFrame;
  int index;
  bool renderText;
  bool renderMain;
  float alpha;
  float alpha2;
  float offset;

  explicit Start(Camera* camFromWindow) : Scene(camFromWindow) {
    alpha = 0.0f;
    alpha2 = 0.0f;
    renderText = true;
    renderMain = false;
    camera->SetDistance(600.0);
    camera->SetIncline(32.0);
    camera->SetPositionTarget(glm::vec3(124.0, 116.0, 6.0));
    timeOnFrame = 0;
    index = 0;
    offset = 0;
    for (int i = 1; i < 33; i++) {
      Texture frame;
      std::string filename = "assets/image/tagguys_open/frame_" +
                             std::to_string(i) + "_delay-0.1s.png";
      frame.init(filename.c_str());
      frames.push_back(frame);
    }

    main.init("assets/UI/IMG_2510 2.PNG");
    credits.init("assets/UI/IMG_2509 2.PNG");
  }

  ~Start() { delete fr; }

  void reset() {
    renderText = true;
    camera->SetDistance(600.0);
    camera->SetIncline(32.0);
    camera->SetPositionTarget(glm::vec3(124.0, 116.0, 6.0));
    timeOnFrame = 0;
    index = 0;
  }

  void draw() override;

  void drawName();

  void update(float delta) override;

  void drawMain();

  void drawCredits();
};
