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
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <string>
#include <vector>

#include "client/graphics/Camera.h"
#include "client/graphics/GameThing.h"
#include "client/graphics/Node.h"
//#include "Light.h"
#include "client/graphics/Collider.h"
#include "client/graphics/Cube.h"
#include "client/graphics/Material.h"
#include "client/graphics/Mesh.h"
#include "client/graphics/Model.h"
#include "client/graphics/Obj.h"
#include "client/graphics/Player.h"
#include "client/graphics/PlayerModel.h"
#include "client/graphics/SkinnedMesh.h"
#include "client/graphics/shader.h"
#include "core/lib.hpp"

#ifndef __SCENE_H__
#define __SCENE_H__

class SceneResourceMap {
 public:
  // The following are containers of object pointers serving as "prefabs" to be
  // referenced across the project.
  std::map<std::string, Skeleton*> skeletons;
  std::map<std::string, Mesh*> meshes;
  std::map<std::string, std::map<std::string, AnimationPlayer*>> animations;
  std::map<std::string, GLuint> shaderPrograms;
  std::map<std::string, Material*> materials;
  std::map<std::string, Texture*> textures;
  std::map<std::string, Model*>
      models;  // more complex; meshes + other info combined
  // std::map< std::string, Light* > light;

  SceneResourceMap() {}

  ~SceneResourceMap() {
    // The containers of pointers own the object pointed to by the pointers.
    // All the objects should be deleted when the object palette ("prefab" list)
    // is destructed. skeletons
    for (auto entry : skeletons) {
      delete entry.second;
    }
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
  }
};

class Scene {
 public:
  static bool _freecam;
  static bool _gizmos;
  static SceneResourceMap _globalSceneResources;

  SceneResourceMap* sceneResources;

  Camera* camera;

  // The container of nodes will be the scene graph after we connect the nodes
  // by setting the child_nodes.
  std::map<std::string, Node*> node;

  std::vector<GameThing*> gamethings;

  Timer time;

  Scene(Camera* camFromWindow) {
    camera = camFromWindow;
    node["_camera"] = camera;
    camera->name = "_camera";
    gamethings.push_back(camera);
    time.time = 300.f; //5 minutes
    time.countdown = true;

    sceneResources = new SceneResourceMap();

    // globals --
    {
      _globalSceneResources.meshes["_gz-cube"] = new Cube();

      _globalSceneResources.meshes["_gz-xyz"] =
          new Obj();  // gizmo for debugging
      _globalSceneResources.meshes["_gz-xyz"]->init("assets/models/_gizmo.obj");

      _globalSceneResources.shaderPrograms["unlit"] =
          LoadShaders("assets/shaders/unlit.vert", "assets/shaders/unlit.frag");

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

      _globalSceneResources.models["_gz-cube"] = new Model;
      _globalSceneResources.models["_gz-cube"]->mesh =
          _globalSceneResources.meshes["_gz-cube"];
      _globalSceneResources.models["_gz-cube"]->material =
          _globalSceneResources.materials["unlit"];
      _globalSceneResources.models["_gz-cube"]->modelMtx =
          glm::translate(glm::vec3(0.0f));
    }
    // --

    // the default scene graph already has one node named "world."
    node["world"] = new Node("world");

    // core!!!
    core_env = new Environment;
    initializeLevel(core_env);
    core_playerpair = initializePlayer();
  }

  void init(void);
  void update(float delta);
  void update_core(
      void);  // TODO(matthew) let game state updates be passed in here

  //void drawHUD(GLFWwindow* window);
  void draw();

  void gui();
  void gui_core();

  // destructor
  ~Scene() {
    // nodes
    for (std::pair<std::string, Node*> entry : node) {
      delete entry.second;
    }

    delete sceneResources;
  }

 private:
  // core
  Environment* core_env;
  std::pair<Player*, ControlModifierData*> core_playerpair;
};

#endif