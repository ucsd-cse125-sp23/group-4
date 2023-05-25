#pragma once
#include <string>
#include <utility>
#include <vector>

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Input.h"
#include "client/graphics/InputListener.h"
#include "client/graphics/Scene.h"

class Lobby : public Scene, public InputListener {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Roboto-Black.ttf");
  Texture background;
  Texture flag;

  Timer wait;
  bool ready;
  bool gameStart;
  float offset;
  std::vector<PlayerModel*> player_models;
  PlayerModel* selectedModel;
  int index;

  explicit Lobby(Camera* camFromWindow) : Scene(camFromWindow) {
    gamethings.clear();
    ready = false;
    index = 0;

    background.init("assets/image/character_select.png");
    flag.init("assets/image/flag.png");

    wait.time = 5;
    wait.countdown = true;
    gameStart = false;
    offset = 0;
  }

  void init(void);
  void update(float delta, UserState& ourPlayerUpdates);

  void buildSceneTree();
  void draw();
  void drawBackground();
  void drawPlayers();
  void lockIn();

  ~Lobby() {
    for (std::pair<std::string, Node*> entry : node) {
      delete entry.second;
    }
    delete sceneResources;
  }
};
