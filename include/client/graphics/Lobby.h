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
  std::map<std::string, Texture> icons;
  std::map<int, message::LobbyPlayer> players;

  Timer wait;
  bool ready;
  bool gameStart;
  float offset;
  std::vector<GameThing*> models;
  std::vector<Model*> player_models;
  Model* selectedModel;
  int index;

  explicit Lobby(Camera* camFromWindow) : Scene(camFromWindow) {
    localGameThings.clear();
    ready = false;
    index = 0;

    background.init("assets/image/character_select.png");
    flag.init("assets/image/flag.png");
    icons["neutral"].init("assets/icons/neutral.png");
    icons["angry"].init("assets/icons/angry.png");
    icons["blushing"].init("assets/icons/blushing.png");
    icons["crying"].init("assets/icons/crying.png");
    icons["heart"].init("assets/icons/heart.png");
    icons["side_eye"].init("assets/icons/side_eye.png");

    wait.time = 5;
    wait.countdown = true;
    gameStart = false;
    offset = 0;
  }

  void init(void);
  void update(float delta) override;
  void receiveState(message::LobbyUpdate newState);
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
