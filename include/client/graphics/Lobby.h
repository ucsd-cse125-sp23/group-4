#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "client/graphics/FontRenderer.h"
#include "client/graphics/Input.h"
#include "client/graphics/InputListener.h"
#include "client/graphics/Scene.h"
#include "client/graphics/Window.h"

class Lobby : public Scene, public InputListener {
 public:
  FontRenderer* fr = new FontRenderer("assets/fonts/Roboto-Black.ttf");
  Texture background;
  Texture flag;
  std::vector<std::string> skin_names{
      "trash panda", "bee", "avocado", "duck", "cat", "unicorn", "waffle"};
  std::map<std::string, Texture> icons;
  std::map<std::string, Texture> ready_icons;
  std::map<int, message::LobbyPlayer> players;

  Timer wait;
  bool is_ready;
  bool gameStart;
  float offset;
  std::vector<GameThing*> models;
  std::vector<Model*> player_models;
  Model* selectedModel;
  int index;

  explicit Lobby(Camera*);
  ~Lobby();

  void init(void) override;
  void update(float delta) override;
  void receiveState(message::LobbyUpdate newState);
  void buildSceneTree();
  void draw() override;
  void drawBackground();
  void drawPlayers();
  void lockIn();
};
