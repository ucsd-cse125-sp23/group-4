#pragma once
#include "client/graphics/Scene.h"
#include "client/graphics/InputListener.h"
#include "client/graphics/Input.h"
#include "client/graphics/FontRenderer.h"

class Lobby : public Scene, public InputListener {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Roboto-Black.ttf");
  Texture background;
  Texture flag;

  bool ready;
  std::vector<PlayerModel*> player_models;
  PlayerModel* selectedModel;
  int index;


  Lobby(Camera* camFromWindow) : Scene(camFromWindow) {
    gamethings.clear();
    ready = false;
    index = 0;

    background.init("assets/image/character_select.png");
    flag.init("assets/image/flag.png");
  }

  void init(void);
  void update(float delta);

  void buildSceneTree();
  void draw();
  void drawBackground();
  void drawPlayers();

  ~Lobby() {
    for (std::pair<std::string, Node*> entry : node) {
      delete entry.second;
    }
    delete sceneResources;
  }
};