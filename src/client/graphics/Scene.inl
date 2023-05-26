/**************************************************
 * Scene.inl
 * contains the definition of the scene graph
 *****************************************************/
#include "client/graphics/Collider.h"
#include "client/graphics/ColliderImporter.h"
#include "client/graphics/Scene.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;

void Scene::init(void) {
  // Create mesh palette
#pragma region Meshes
  // TODO(eddie) use Assimp here...
  sceneResources->meshes["playermodel"] = new Obj();
  sceneResources->meshes["playermodel"]->init(
      "assets/models/model-skeleton.obj");

  sceneResources->meshes["teapot"] = new Obj();
  sceneResources->meshes["teapot"]->init("assets/models/teapot.obj");

  sceneResources->meshes["cubeUV"] = new Obj();
  sceneResources->meshes["cubeUV"]->init(
      "assets/models/bevel_cube.obj");  // includes UV data
#pragma endregion

  // Create shader palette
#pragma region Shaders
  sceneResources->shaderPrograms["basic"] =
      LoadShaders("assets/shaders/shader.vert", "assets/shaders/shaderx.frag");
  sceneResources->shaderPrograms["toon"] =
      LoadShaders("assets/shaders/shader.vert", "assets/shaders/toon.frag");
#pragma endregion

  // Create texture palette
#pragma region Textures
  sceneResources->textures["grid"] = new Texture;
  sceneResources->textures["grid"]->init("assets/image/test_uv.png");
#pragma endregion

  // Create material palette
#pragma region Materials
  sceneResources->materials["wood"] = new Material;
  sceneResources->materials["wood"]->shader =
      sceneResources->shaderPrograms["basic"];
  sceneResources->materials["wood"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  sceneResources->materials["wood"]->diffuse = vec4(0.4f, 0.15f, 0.1f, 1.0f);
  sceneResources->materials["wood"]->specular = vec4(0.3f, 0.15f, 0.1f, 1.0f);
  sceneResources->materials["wood"]->shininess = 100.0f;

  sceneResources->materials["ceramic"] = new Material;
  sceneResources->materials["ceramic"]->shader =
      sceneResources->shaderPrograms["basic"];
  sceneResources->materials["ceramic"]->ambient =
      vec4(0.02f, 0.07f, 0.2f, 1.0f);
  sceneResources->materials["ceramic"]->diffuse = vec4(0.1f, 0.25f, 0.7f, 1.0f);
  sceneResources->materials["ceramic"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
  sceneResources->materials["ceramic"]->shininess = 50.0f;

  sceneResources->materials["silver"] = new Material;
  sceneResources->materials["silver"]->shader =
      sceneResources->shaderPrograms["basic"];
  sceneResources->materials["silver"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  sceneResources->materials["silver"]->diffuse = vec4(0.2f, 0.2f, 0.2f, 1.0f);
  sceneResources->materials["silver"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
  sceneResources->materials["silver"]->shininess = 50.0f;

  sceneResources->materials["marble"] = new Material;
  sceneResources->materials["marble"]->shader =
      sceneResources->shaderPrograms["basic"];
  sceneResources->materials["marble"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  sceneResources->materials["marble"]->diffuse =
      vec4(0.97f, 0.89f, 0.75f, 1.0f);
  sceneResources->materials["marble"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
  sceneResources->materials["marble"]->shininess = 50.0f;

  sceneResources->materials["grid"] = new Material;
  sceneResources->materials["grid"]->shader =
      sceneResources->shaderPrograms["basic"];
  sceneResources->materials["grid"]->texture = sceneResources->textures["grid"];
  sceneResources->materials["grid"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  sceneResources->materials["grid"]->diffuse = vec4(0.3f, 0.3f, 0.3f, 1.0f);

  sceneResources->materials["toon.blue"] = new Material;
  sceneResources->materials["toon.blue"]->shader =
      sceneResources->shaderPrograms["toon"];
  sceneResources->materials["toon.blue"]->texture =
      sceneResources->textures["grid"];
  sceneResources->materials["toon.blue"]->ambient =
      vec4(0.1f, 0.1f, 0.1f, 1.0f);
  sceneResources->materials["toon.blue"]->diffuse =
      vec4(0.6f, 0.6f, 0.97f, 1.0f);
  sceneResources->materials["toon.blue"]->specular =
      vec4(0.9f, 0.9f, 0.9f, 1.0f);
  sceneResources->materials["toon.blue"]->shininess = 50.0f;
#pragma endregion

  // Create model palette
#pragma region Models
  sceneResources->models["teapot1"] = new Model;
  sceneResources->models["teapot1"]->mesh = sceneResources->meshes["teapot"];
  sceneResources->models["teapot1"]->material =
      sceneResources->materials["toon.blue"];
  sceneResources->models["teapot1"]->modelMtx = scale(vec3(1.5f));

  sceneResources->models["cubeTextured"] = new Model;
  sceneResources->models["cubeTextured"]->mesh =
      sceneResources->meshes["cubeUV"];
  sceneResources->models["cubeTextured"]->material =
      sceneResources->materials["grid"];
#pragma endregion

  // Import sound palette
#pragma region Sounds
  SoundEffect* sfx = new SoundEffect();
  sceneResources->sounds["test"] = sfx;
  sfx->load("assets/sounds/sound_test.wav");
#pragma endregion

  // Initialize map data
#pragma region MapImport
  ///// maps:
  sceneResources->meshes["mapA"] = new Obj();
  sceneResources->meshes["mapA"]->init("assets/models/map_testing.obj");
  sceneResources->models["mapA"] = new Model;
  sceneResources->models["mapA"]->mesh = sceneResources->meshes["mapA"];
  sceneResources->models["mapA"]->material =
      sceneResources->materials["marble"];
  sceneResources->models["mapA"]->modelMtx = translate(vec3(0, -2, 0));

  sceneResources->meshes["mapColsTesting"] = new Obj();
  sceneResources->meshes["mapColsTesting"]->init(
      "assets/models/test_colliders.obj");  // multiple objects in one file
  sceneResources->models["mapColsTesting"] = new Model;
  sceneResources->models["mapColsTesting"]->mesh =
      sceneResources->meshes["mapColsTesting"];
  sceneResources->models["mapColsTesting"]->material =
      sceneResources->materials["marble"];
  // sceneResources->models["mapColsTesting"]->transformMtx = translate(vec3(0,
  // -2, 0));   // needs to be world space

  ///// collisions:
  std::vector<ColliderData> mapColliders =
      ColliderImporter::ImportCollisionData("assets/models/test_colliders.obj");

  node["collision"] = new Node("_colliders");
  for (auto c : mapColliders) {
    node["collision"]->childnodes.push_back(new Collider(c));
  }
  // node["collision"]->transformMtx =
  // sceneResources->models["mapColsTesting"]->transformMtx;
#pragma endregion

  // Setup player/gameplay prefabs
#pragma region Prefabs
  Model* m_prefab = new Model;
  sceneResources->models["PREFAB_player.model"] = m_prefab;
  m_prefab->mesh = sceneResources->meshes["playermodel"];
  // TODO(matthew) copy over mesh too? for animations?
  m_prefab->material = sceneResources->materials["toon.blue"];
#pragma endregion

  ///////////////////////////////////////////////////////
  printf("\nScene: done loading resources!\n");
  ///////////////////////////////////////////////////////

  // Add local game things
  GameThing* thing_example = new GameThing;
  thing_example->name = "GT_teapot";
  gamethings.push_back(thing_example);

  GameThing* thing_cube = new GameThing;
  thing_cube->name = "GT_cube";
  gamethings.push_back(thing_cube);

  // Build the scene graph
  node["teapot1"] = thing_example;
  node["cubeTest"] = thing_cube;

  node["map"] = new Node("_map-test");
  node["map"]->model = sceneResources->models["mapColsTesting"];

  thing_example->transform.position = vec3(-2.0f, 2.0f, 5.0f);  // gt only
  node["teapot1"]->model = sceneResources->models["teapot1"];

  thing_cube->transform.position = vec3(2.0f, 2.0f, 5.0f);
  node["cubeTest"]->model = sceneResources->models["cubeTextured"];
  node["cubeTest"]->transformMtx = translate(vec3(2, 2, 5));  // node translate

  // Push scene nodes to root node
  // ("world" node already created in Scene constructor)

  // node["world"]->childnodes.push_back(node["teapot1"]);
  // node["world"]->childnodes.push_back(node["cubeTest"]);

  node["world"]->childnodes.push_back(node["collision"]);

  node["world"]->childnodes.push_back(node["map"]);
}
