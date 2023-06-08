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
#include "client/graphics/Material.h"
#include "client/graphics/Mesh.h"
#include "client/graphics/Model.h"
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
#include "client/graphics/SoundEffect.h"
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

  Camera* camera;
  Player* myPlayer = nullptr;

  // The container of nodes will be the scene graph after we connect the nodes
  // by setting the child_nodes.
  // by setting the child_nodes.
  std::map<std::string, Node*> node;

  std::vector<GameThing*> localGameThings;
  std::unordered_map<int, GameThing*> networkGameThings;
  std::map<int, std::string> skins;

  Timer time;
  bool gameStart;
  SoundEffect* gameTheme;
  
  explicit Scene(Camera* camFromWindow) {
    camera = camFromWindow;
    node["_camera"] = camera;
    camera->name = "_camera";
    localGameThings.push_back(camera);
    time.time = 300.0f;
    time.countdown = true;
    gameStart = false;

    sceneResources = new SceneResourceMap();

    // globals --
    _globalSceneResources.meshes["_gz-cube"] = new Cube();

    _globalSceneResources.meshes["_gz-xyz"] = new Obj();  // gizmo for debugging
    _globalSceneResources.meshes["_gz-xyz"]->init(
        "assets/model/dev/_gizmo.obj");

    _globalSceneResources.shaderPrograms["unlit"] =
        LoadShaders("assets/shaders/shader.vert", "assets/shaders/unlit.frag");

    _globalSceneResources.materials["unlit"] = new Material;
    _globalSceneResources.materials["unlit"]->shader =
        _globalSceneResources.shaderPrograms["unlit"];
    _globalSceneResources.materials["unlit"]->diffuse =
        glm::vec4(0.99f, 0.0f, 0.86f, 1.0f);

    _globalSceneResources.models["_gz-xyz"] = new Model;
    _globalSceneResources.models["_gz-xyz"]->mesh =
        _globalSceneResources.meshes["_gz-xyz"];
    _globalSceneResources.models["_gz-xyz"]->material =
        _globalSceneResources.materials["unlit"];
    _globalSceneResources.models["_gz-xyz"]->modelMtx =
        glm::scale(glm::vec3(1.0f));
    _globalSceneResources.models["_gz-xyz"]->depthFunction = GL_ALWAYS;

    _globalSceneResources.models["_gz-cube"] = new Model;
    _globalSceneResources.models["_gz-cube"]->mesh =
        _globalSceneResources.meshes["_gz-cube"];
    _globalSceneResources.models["_gz-cube"]->material =
        _globalSceneResources.materials["unlit"];
    _globalSceneResources.models["_gz-cube"]->modelMtx =
        glm::translate(glm::vec3(0.0f));
    _globalSceneResources.models["_gz-cube"]->depthFunction = GL_ALWAYS;
    // --

    // the default scene graph already has one node named "world."
    node["world"] = new Node("world");
  }

  Player* createPlayer(int id);
  void removePlayer(int id);
  void initFromServer(int myid);
  void setToUserFocus(GameThing* t);
  virtual void init(void);
  void init(std::map<int, message::LobbyPlayer> players);

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

    delete sceneResources;
  }
};
