#pragma once

#include "client/graphics/Scene.h"
#include "client/graphics/InputListener.h"
#include "client/graphics/Input.h"

class Lobby : public Scene, public InputListener {
 public:
  Texture background;

  int index;

  bool ready;
  std::vector<PlayerModel*> player_models;
  PlayerModel* selectedModel;

  Lobby(Camera* camFromWindow) : Scene(camFromWindow) {
    gamethings.clear();
    ready = false;
    index = 0;

    background.init("assets/image/test_uv.png");
  }

  void init(void);
  void update(float delta);

  void buildSceneTree();
  void draw();
  void drawBackground();

  ~Lobby() {
    for (std::pair<std::string, Node*> entry : node) {
      delete entry.second;
    }
    delete sceneResources;
  }
};