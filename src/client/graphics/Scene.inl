/**************************************************
 * Scene.inl
 * contains the definition of the scene graph
 *****************************************************/
#include "client/graphics/Collider.h"
#include "client/graphics/MapDataImporter.h"
#include "client/graphics/MapObj.h"
#include "client/graphics/Scene.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <config/lib.hpp>

using namespace glm;

void Scene::init(void) {
  auto config = get_config();

  // Create mesh palette
#pragma region Meshes
  sceneResources->meshes["playermodel"] = new Obj();
  sceneResources->meshes["playermodel"]->init(
      "assets/model/dev/model-skeleton.obj");

  sceneResources->meshes["teapot"] = new Obj();
  sceneResources->meshes["teapot"]->init(
      "assets/model/dev/imported/teapot.obj");

  sceneResources->meshes["cubeUV"] = new Obj();
  sceneResources->meshes["cubeUV"]->init(
      "assets/model/dev/bevel_cube.obj");  // includes UV data

  sceneResources->meshes["particleQuad"] = new Obj();
  sceneResources->meshes["particleQuad"]->init("assets/model/quad.obj");
#pragma endregion

  // Create shader palette
#pragma region Shaders
  sceneResources->shaderPrograms["basic"] =
      LoadShaders("assets/shaders/shader.vert", "assets/shaders/shaderx.frag");
  sceneResources->shaderPrograms["unlitx"] =
      LoadShaders("assets/shaders/shader.vert", "assets/shaders/unlitx.frag");
  sceneResources->shaderPrograms["toon"] =
      LoadShaders("assets/shaders/shader.vert", "assets/shaders/toon.frag");
#pragma endregion

  // Create texture palette
#pragma region Textures
  sceneResources->textures["grid"] = new Texture;
  sceneResources->textures["grid"]->init("assets/image/test_uv.png");

  sceneResources->textures["stars"] = new Texture;
  sceneResources->textures["stars"]->init("assets/image/tile_stars.png");

  sceneResources->textures["stars-ptcl"] = new Texture;
  sceneResources->textures["stars-ptcl"]->init(
      "assets/image/particle_stars.png");
#pragma endregion

  // Create material palette
#pragma region Materials
  Material* mat;

  sceneResources->materials["wood"] = new Material;
  sceneResources->materials["wood"]->shader =
      sceneResources->shaderPrograms["basic"];
  sceneResources->materials["wood"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  sceneResources->materials["wood"]->diffuse = vec4(0.4f, 0.15f, 0.1f, 1.0f);
  sceneResources->materials["wood"]->specular = vec4(0.3f, 0.15f, 0.1f, 1.0f);
  sceneResources->materials["wood"]->shininess = 100.0f;

  mat = new Material;  // leaf
  mat->shader = sceneResources->shaderPrograms["basic"];
  mat->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->diffuse = vec4(0.3f, 0.6f, 0.3f, 1.0f);
  mat->specular = vec4(0.15f, 0.15f, 0.1f, 1.0f);
  mat->shininess = 100.0f;
  sceneResources->materials["l"] = mat;

  mat = new Material;  // stem
  mat->shader = sceneResources->shaderPrograms["basic"];
  mat->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->diffuse = vec4(0.66f, 0.77f, 0.6f, 1.0f);
  mat->specular = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->shininess = 100.0f;
  sceneResources->materials["stem"] = mat;

  mat = new Material;  // tree trunk wood
  mat->shader = sceneResources->shaderPrograms["basic"];
  mat->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->diffuse = vec4(0.46f, 0.15f, 0.1f, 1.0f);
  mat->specular = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->shininess = 100.0f;
  sceneResources->materials["treetrunk"] = mat;

  mat = new Material;  // river water
  mat->shader = sceneResources->shaderPrograms["basic"];
  mat->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->diffuse = vec4(0.16f, 0.15f, 0.9f, 1.0f);
  mat->specular = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->shininess = 100.0f;
  sceneResources->materials["river"] = mat;

  mat = new Material;  // cloud
  mat->shader = sceneResources->shaderPrograms["basic"];
  mat->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->diffuse = vec4(0.99f, 0.9f, 0.9f, 1.0f);
  mat->specular = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->shininess = 200.0f;
  sceneResources->materials["Cloud"] = mat;

  mat = new Material;  // ground
  mat->shader = sceneResources->shaderPrograms["basic"];
  mat->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->diffuse = vec4(0.99f, 0.6f, 0.3f, 1.0f);
  mat->specular = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->shininess = 100.0f;
  sceneResources->materials["ground"] = mat;

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

  sceneResources->materials["pyramid"] = new Material;
  sceneResources->materials["pyramid"]->shader =
      sceneResources->shaderPrograms["basic"];
  sceneResources->materials["pyramid"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  sceneResources->materials["pyramid"]->diffuse = vec4(0.7f, 0.7f, 0.2f, 1.0f);
  sceneResources->materials["pyramid"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
  sceneResources->materials["pyramid"]->shininess = 50.0f;

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
  sceneResources->materials["grid"]->diffuse = vec4(0.7f, 0.7f, 0.7f, 1.0f);

  Material* mtl = new Material;
  mtl->shader = sceneResources->shaderPrograms["unlitx"];
  mtl->texture = sceneResources->textures["stars-ptcl"];
  mtl->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mtl->diffuse = vec4(0.9f, 0.82f, 0.0f, 1.0f);
  sceneResources->materials["stars-ptcl"] = mtl;

  sceneResources->materials["toon.blue"] = new Material;
  sceneResources->materials["toon.blue"]->shader =
      sceneResources->shaderPrograms["toon"];
  sceneResources->materials["toon.blue"]->texture =
      sceneResources->textures["stars"];
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

  // Setup particle effects
#pragma region Particles
  ParticleSystem* ptcl = new ParticleSystem();
  ptcl->name = "GT_particle1";
  ptcl->meshRef = sceneResources->meshes["particleQuad"];
  ptcl->materialRef = sceneResources->materials["stars-ptcl"];
  ptcl->transform.position = vec3(0, 3, 0);
  ptcl->transform.updateMtx(&ptcl->transformMtx);
  localGameThings.push_back(ptcl);
  node["particleTest"] = ptcl;
  node["world"]->childnodes.push_back(node["particleTest"]);

  ptcl = new ParticleSystem();
  ptcl->name = "GT_particle2";
  ptcl->meshRef = sceneResources->meshes["particleQuad"];
  ptcl->materialRef = sceneResources->materials["stars-ptcl"];
  ptcl->transform.position = vec3(10, 9, 3);
  ptcl->transform.updateMtx(&ptcl->transformMtx);
  localGameThings.push_back(ptcl);
  node["particleTest2"] = ptcl;
  node["world"]->childnodes.push_back(node["particleTest2"]);
#pragma endregion

  // Skybox setup
#pragma region Skybox
  sceneResources->shaderPrograms["skybox"] =
      LoadShaders("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");

  sceneResources->meshes["skybox"] = new Skybox();
  sceneResources->meshes["skybox"]->init();

  TextureCube* skyboxCubemapTexture = new TextureCube();
  skyboxCubemapTexture->init();
  sceneResources->textures["skyboxCubemap"] = skyboxCubemapTexture;

  sceneResources->materials["skybox"] = new Material;
  sceneResources->materials["skybox"]->shader =
      sceneResources->shaderPrograms["skybox"];
  sceneResources->materials["skybox"]->texture = skyboxCubemapTexture;

  sceneResources->models["skybox"] = new Model;
  sceneResources->models["skybox"]->mesh = sceneResources->meshes["skybox"];
  sceneResources->models["skybox"]->material =
      sceneResources->materials["skybox"];
  sceneResources->models["skybox"]->depthFunction = GL_LEQUAL;
#pragma endregion

  // Initialize map data
#pragma region MapImport
  ///// maps:
  MapObj* mapVisuals = new MapObj();
  sceneResources->models["map"] = mapVisuals;
  mapVisuals->init(config["map_draw_file"], sceneResources->materials);
  mapVisuals->material = sceneResources->materials["marble"];

  ///// map data (collisions, fx, etc.):
  MapData mapData = MapDataImporter::Import(config["map_data_file"]);

  node["collision"] = new Node("_colliders");
  for (auto c : mapData.colliders) {
    node["collision"]->childnodes.push_back(new Collider(c));
  }
#pragma endregion

  // Setup player/gameplay prefabs
#pragma region Prefabs
  Model* m_prefab = new Model;
  sceneResources->models["PREFAB_player.model3"] = m_prefab;
  m_prefab->mesh = sceneResources->meshes["playermodel"];
  // TODO(matthew) copy over mesh too? for animations?
  m_prefab->material = sceneResources->materials["toon.blue"];

  AssimpModel* am = new AssimpModel();
  am->loadAssimp("assets/animation/withUV/Animation -Bee.fbx");
  am->setAnimation("walk");
  sceneResources->models["PREFAB_player.model"] = am;
  sceneResources->models["PREFAB_player.model"]->mesh = am;
  sceneResources->models["PREFAB_player.model"]->material =
      sceneResources->materials["wood"];
#pragma endregion

  ///////////////////////////////////////////////////////
  printf("\nScene: done loading resources!\n");
  ///////////////////////////////////////////////////////

  // Add local game things
  GameThing* thing_example = new GameThing;
  thing_example->name = "GT_teapot";
  localGameThings.push_back(thing_example);

  GameThing* thing_cube = new GameThing;
  thing_cube->name = "GT_cube";
  localGameThings.push_back(thing_cube);

  // Build nodes of the scene graph
  node["teapot1"] = thing_example;
  node["cubeTest"] = thing_cube;

  node["map"] = new Node("_map");
  node["map"]->model = sceneResources->models["map"];

  thing_example->transform.position = vec3(-2.0f, 2.0f, 5.0f);  // gt only
  node["teapot1"]->model = sceneResources->models["teapot1"];

  thing_cube->transform.position = vec3(2.0f, 2.0f, 5.0f);
  node["cubeTest"]->model = sceneResources->models["cubeTextured"];
  node["cubeTest"]->transformMtx = translate(vec3(2, 2, 5));  // node translate

  node["skybox"] = new Node("_skybox");
  node["skybox"]->model = sceneResources->models["skybox"];
  node["skybox"]->skybox = true;

  // Push scene nodes to root node
  // ("world" node already created in Scene constructor)

  node["world"]->childnodes.push_back(node["skybox"]);

  // node["world"]->childnodes.push_back(node["teapot1"]);
  node["world"]->childnodes.push_back(node["cubeTest"]);

  node["world"]->childnodes.push_back(node["collision"]);

  node["world"]->childnodes.push_back(node["map"]);

  //createPlayer(-1);
}
