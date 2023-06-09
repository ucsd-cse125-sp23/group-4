/**************************************************
 * Scene.h
 * stores all allocated resource objects and constructs
 * the game scene hierarchy via Scene.inl
 *****************************************************/
#pragma once

#define GLM_FORCE_RADIANS
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>
#include <glm/gtx/transform.hpp>
#include <map>
#include <network/message.hpp>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "client/graphics/AssimpModel.h"
#include "client/graphics/Camera.h"
#include "client/graphics/Cube.h"
#include "client/graphics/GameThing.h"
#include "client/graphics/ItemBox.h"
#include "client/graphics/Leaderboard.h"
#include "client/graphics/Material.h"
#include "client/graphics/Mesh.h"
#include "client/graphics/Model.h"
#include "client/graphics/Music.h"
#include "client/graphics/Node.h"
#include "client/graphics/Obj.h"
#include "client/graphics/ParticleSystem.h"
#include "client/graphics/Player.h"
#include "client/graphics/PlayerModel.h"
#include "client/graphics/Skybox.h"
#include "client/graphics/SoundEffect.h"
#include "client/graphics/Texture.h"
#include "client/graphics/TextureCube.h"
#include "client/graphics/Timer.h"
#include "client/graphics/shader.h"
// #include "core/game/level/Environment.h"

using namespace client;  // NOLINT

class SceneResourceMap {
 public:
  // The following are containers of object pointers serving as "prefabs" to be
  // referenced across the project.
  std::map<std::string, Mesh*> meshes;
  std::map<std::string, GLuint> shaderPrograms;
  std::map<std::string, Material*> materials;
  std::map<std::string, Texture*> textures;
  std::map<std::string, Model*>
      models;  // more complex; meshes + other info combined
  std::map<std::string, SoundEffect*> sounds;
  std::map<std::string, GameThing*> prefabs;
  // std::map< std::string, Light* > light;

  SceneResourceMap() {}

  ~SceneResourceMap() {
    // The containers of pointers own the object pointed to by the pointers.
    // All the objects should be deleted when the object palette ("prefab" list)
    // is destructed.
    // mesh geometry
    for (auto entry : meshes) {
      delete entry.second;
    }
    for (auto entry : shaderPrograms) {
      glDeleteProgram(entry.second);
    }
    // materials
    for (auto entry : materials) {
      delete entry.second;
    }
    // textures
    for (auto entry : textures) {
      delete entry.second;
    }
    // models
    for (auto entry : models) {
      delete entry.second;
    }
    // sounds
    for (auto entry : sounds) {
      delete entry.second;
    }
    // prefabs
    for (auto entry : prefabs) {
      delete entry.second;
    }
  }
};

class Scene {
 public:
  static int _myPlayerId;
  static bool _freecam;
  static bool _gizmos;
  static SceneResourceMap _globalSceneResources;

  SceneResourceMap* sceneResources;

  Environment* coreEnv;

  Camera* camera;
  Player* myPlayer = nullptr;

  // The container of nodes will be the scene graph after we connect the nodes
  // by setting the child_nodes.
  // by setting the child_nodes.
  std::map<std::string, Node*> node;

  std::vector<GameThing*> localGameThings;
  std::unordered_map<int, GameThing*> networkGameThings;
  std::map<int, std::string> skins;
  std::vector<std::string> rankings;
  std::vector<Player*> rankings_ptr;

  Timer time;
  bool gameStart;
  float timeOver;
  Leaderboard leaderboard;
  Music* music;

  explicit Scene(Camera* camFromWindow) {
    coreEnv = new Environment();
    // initializeLevel(coreEnv);    // not needed

    camera = camFromWindow;
    // camera->env = coreEnv;    // raycasts: uncomment this (its broken)
    node["_camera"] = camera;
    camera->name = "_camera";
    localGameThings.push_back(camera);

    time.time = 5.0f;
    time.countdown = true;
    gameStart = false;
    timeOver = 0;

    sceneResources = new SceneResourceMap();
    // the default scene graph already has one node named "world."
    node["world"] = new Node("world");
    music = new Music();
    music->load("assets/sounds/Dance_Powder.wav");
  }

  Player* createPlayer(int id, std::string skin);
  void removePlayer(int id);
  void initFromServer(int myid);
  void setToUserFocus(GameThing* t);
  ItemBox* createItemBox(int id, Item iEnum);
  void removeItemBox(int id);
  virtual void init(void);
  void init(std::map<int, message::LobbyPlayer> players);
  virtual void reset();
  std::vector<std::string> rankPlayers();

  message::UserStateUpdate pollUpdate();                 // broadcast to net
  void receiveState(message::GameStateUpdate newState);  // receive from net

  // received from net:
  void receiveEvent_jump(message::JumpEvent e);
  void receiveEvent_land(message::LandEvent e);
  void receiveEvent_item(message::ItemPickupEvent e);
  void receiveEvent_tag(message::TagEvent e);

  virtual void animate(float delta);
  virtual void update(float delta);

  virtual void draw();

  void gui();

  // destructor
  ~Scene() {
    // nodes
    for (std::pair<std::string, Node*> entry : node) {
      delete entry.second;
    }
    if (music) delete music;
    delete sceneResources;

    delete coreEnv;
  }

 private:
  const double min_time_between_animate = 1.0 / 15;
  float fpsCapParam = 15;
  double num_updates_to_send = 0;
};
