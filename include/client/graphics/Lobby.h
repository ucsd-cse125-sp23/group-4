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
  std::vector<std::string> skin_names;
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
    icons["blushing"].init("assets/icons/blushing.png");
    icons["crying"].init("assets/icons/crying.png");
    icons["side_eye"].init("assets/icons/side_eye.png");

    skin_names.push_back("neutral");
    skin_names.push_back("blushing");
    skin_names.push_back("crying");
    skin_names.push_back("side_eye");

    wait.time = 5;
    wait.countdown = true;
    gameStart = false;
    offset = 0;
  }

  void init(void);
  void update(float delta, GamePhase& phase, bool& transition);
  void receiveState(message::LobbyUpdate newState);
  message::LobbyPlayerUpdate pollUpdate();
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
