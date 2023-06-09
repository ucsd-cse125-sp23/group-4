/**************************************************
 * Scene.inl
 * contains the definition of the scene graph
 *****************************************************/
#include "client/graphics/Collider.h"
#include "client/graphics/DOF.h"
#include "client/graphics/MapDataImporter.h"
#include "client/graphics/MapObj.h"
#include "client/graphics/Scene.h"
#include "client/graphics/Window.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <config/lib.hpp>

using namespace glm;

void Scene::init(void) {
  music = new Music();
  music->load("assets/sounds/Dance_Powder.wav");

  // globals --
  _globalSceneResources.meshes["_gz-cube"] = new Cube();

  _globalSceneResources.meshes["_gz-xyz"] = new Obj();  // gizmo for debugging
  _globalSceneResources.meshes["_gz-xyz"]->init("assets/model/dev/_gizmo.obj");

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
  sceneResources->shaderPrograms["map"] =
      LoadShaders("assets/shaders/shader.vert", "assets/shaders/shaderx.frag");
  sceneResources->shaderPrograms["water"] =
      LoadShaders("assets/shaders/shader.vert",
                  "assets/shaders/shaderx.frag");  // TODO(gfx team?)
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

  sceneResources->textures["star1-ptcl"] = new Texture;
  sceneResources->textures["star1-ptcl"]->init(
      "assets/image/particle_star1.png");
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

  mat = new Material;  // custom configured map material
  mat->shader = sceneResources->shaderPrograms["basic"];
  mat->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mat->diffuse = vec4(0.3f, 0.6f, 0.3f, 1.0f);
  mat->specular = vec4(0.15f, 0.15f, 0.1f, 1.0f);
  mat->shininess = 100.0f;
  sceneResources->materials["mapMaterialExample"] = mat;

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

  sceneResources->materials["shine.red"] = new Material;
  sceneResources->materials["shine.red"]->shader =
      sceneResources->shaderPrograms["basic"];
  sceneResources->materials["shine.red"]->ambient =
      vec4(0.1f, 0.1f, 0.1f, 1.0f);
  sceneResources->materials["shine.red"]->diffuse =
      vec4(0.9f, 0.2f, 0.0f, 1.0f);
  sceneResources->materials["shine.red"]->specular =
      vec4(0.0f, 0.95f, 0.0f, 1.0f);
  sceneResources->materials["shine.red"]->emission =
      vec4(0.5f, 0.0f, 0.0f, 1.0f);
  sceneResources->materials["shine.red"]->shininess = 50.0f;

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

  mtl = new Material;
  mtl->shader = sceneResources->shaderPrograms["unlitx"];
  mtl->texture = sceneResources->textures["star1-ptcl"];
  mtl->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mtl->diffuse = vec4(0.9f, 0.82f, 0.0f, 1.0f);
  sceneResources->materials["star1-ptcl"] = mtl;

  mtl = new Material;
  mtl->shader = sceneResources->shaderPrograms["unlitx"];
  mtl->texture = sceneResources->textures["star1-ptcl"];
  mtl->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mtl->diffuse = vec4(0.1f, 0.4f, 0.9f, 1.0f);
  sceneResources->materials["star1.blue-ptcl"] = mtl;

  mtl = new Material;
  mtl->shader = sceneResources->shaderPrograms["toon"];
  mtl->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
  mtl->diffuse = vec4(0.9f, 0.82f, 0.9f, 1.0f);
  sceneResources->materials["toon"] = mtl;

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

  sceneResources->models["cubeBoxTest"] = new Model;
  sceneResources->models["cubeBoxTest"]->mesh =
      sceneResources->meshes["cubeUV"];
  sceneResources->models["cubeBoxTest"]->material =
      sceneResources->materials["shine.red"];
#pragma endregion

#pragma region Sounds
  // Sound palette
  SoundEffect* sfx = new SoundEffect();
  sfx->load("assets/sounds/jump.wav");
  sceneResources->sounds["test"] = sfx;
  sfx = new SoundEffect();
  sfx->load("assets/sounds/jump2.wav");
  sceneResources->sounds["sfx_jump"] = sfx;
  sfx = new SoundEffect();
  sfx->load("assets/sounds/hurt.wav");
  sceneResources->sounds["sfx_tag"] = sfx;
  sfx = new SoundEffect();
  sfx->load("assets/sounds/coin.wav");
  sceneResources->sounds["sfx_item"] = sfx;
  sfx = new SoundEffect();
  sfx->load("assets/sounds/fallsound.wav");
  sceneResources->sounds["sfx_fall"] = sfx;
  /*music = new Music();
  music->load("assets/sounds/Dance_Powder.wav");*/

#pragma endregion

  // Setup particle effects
#pragma region Particles
  ParticleSystem* ptcl = new ParticleSystem();
  ptcl->name = "GT_particle_test";
  ptcl->meshRef = sceneResources->meshes["particleQuad"];
  ptcl->materialRef = sceneResources->materials["stars-ptcl"];
  ptcl->transform.position = vec3(0, 3, 0);
  ptcl->transform.updateMtx(&ptcl->transformMtx);
  // localGameThings.push_back(ptcl);
  // node["particleTest"] = ptcl;
  // node["world"]->childnodes.push_back(node["particleTest"]);

  ptcl = new ParticleSystem();
  ptcl->name = "GTptcl_stars";
  ptcl->meshRef = sceneResources->meshes["particleQuad"];
  ptcl->materialRef = sceneResources->materials["star1-ptcl"];
  ptcl->worldSpace = true;
  ptcl->creationRate = 0;
  ptcl->initVelocity = {DOFr(-10, 10), DOFr(-10, 10), DOFr(-10, 10)};
  sceneResources->prefabs["ptcl_stars"] = ptcl;

  ptcl = new ParticleSystem();
  ptcl->name = "GTptcl_jump";
  ptcl->meshRef = sceneResources->meshes["particleQuad"];
  ptcl->materialRef = sceneResources->materials["star1-ptcl"];
  ptcl->worldSpace = true;
  ptcl->creationRate = 0;
  ptcl->initVelocity = {DOFr(-10, 10), DOFr(12), DOFr(-10, 10)};
  ptcl->gravity.SetValue(-0.01f);
  ptcl->drag.SetValue(5.0f);
  ptcl->lifespan.SetMinMax(0.9f, 0.95f);
  sceneResources->prefabs["ptcl_jump"] = ptcl;

  ptcl = new ParticleSystem();
  ptcl->name = "GTptcl_isTagged";
  ptcl->meshRef = sceneResources->meshes["particleQuad"];
  ptcl->materialRef = sceneResources->materials["star1.blue-ptcl"];
  ptcl->worldSpace = true;
  ptcl->creationRate = 0;
  ptcl->lifespan.SetMinMax(0.6f, 0.8f);
  ptcl->gravity.SetValue(-0.1f);
  sceneResources->prefabs["ptcl_isTagged"] = ptcl;
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
  mapVisuals->init(config["map_draw_file"], sceneResources->materials,
                   sceneResources->shaderPrograms["map"]);
  mapVisuals->material = sceneResources->materials["marble"];

  ///// map data (collisions, fx, etc.):
  MapData mapData = MapDataImporter::Import(config["map_data_file"]);

  node["collision"] = new Node("_colliders");
  for (auto c : mapData.colliders) {
    node["collision"]->childnodes.push_back(new Collider(c));

    if (coreEnv) coreEnv->addConvex(c.vertices);  // used by camera raycasts
  }
  if (coreEnv) coreEnv->constructBVH();
#pragma endregion

    // Setup player/gameplay prefabs
#pragma region Prefabs
  Model* m_prefab = new Model;
  sceneResources->models["PREFAB_player.model3"] = m_prefab;
  m_prefab->mesh = sceneResources->meshes["playermodel"];
  // TODO(matthew) copy over mesh too? for animations?
  m_prefab->material = sceneResources->materials["toon.blue"];

  AssimpModel* am = new AssimpModel();
  am->loadAssimp("assets/animation/AnimExtra-BeeTest.fbx");
  am->setAnimation("walk");
  sceneResources->models["PREFAB_player.model"] = am;
  sceneResources->models["PREFAB_player.model"]->mesh = am;
  sceneResources->models["PREFAB_player.model"]->material =
      sceneResources->materials["toon"];

  AssimpModel* racoon_prefab = new AssimpModel();
  racoon_prefab->loadAssimp("assets/animation/withUV/Animation -Racoon.fbx");
  racoon_prefab->setAnimation("walk");
  sceneResources->models["trash panda"] = racoon_prefab;
  sceneResources->models["trash panda"]->mesh = racoon_prefab;
  sceneResources->models["trash panda"]->material =
      sceneResources->materials["toon"];

  AssimpModel* bee_prefab = new AssimpModel();
  bee_prefab->loadAssimp("assets/animation/withUV/Animation -Bee.fbx");
  bee_prefab->setAnimation("walk");
  sceneResources->models["bee"] = bee_prefab;
  sceneResources->models["bee"]->material = sceneResources->materials["toon"];
  sceneResources->models["bee"]->mesh = bee_prefab;

  AssimpModel* cat_prefab = new AssimpModel();
  cat_prefab->loadAssimp("assets/animation/withUV/Animation -Cat.fbx");
  cat_prefab->setAnimation("walk");
  sceneResources->models["cat"] = cat_prefab;
  sceneResources->models["cat"]->material = sceneResources->materials["toon"];
  sceneResources->models["cat"]->mesh = bee_prefab;

  AssimpModel* avocado_prefab = new AssimpModel();
  avocado_prefab->loadAssimp("assets/animation/withUV/Animation -Avocado.fbx");
  avocado_prefab->setAnimation("walk");
  sceneResources->models["avocado"] = avocado_prefab;
  sceneResources->models["avocado"]->mesh = avocado_prefab;
  sceneResources->models["avocado"]->material =
      sceneResources->materials["toon"];

  AssimpModel* duck_prefab = new AssimpModel();
  duck_prefab->loadAssimp("assets/animation/withUV/Animation -Duck.fbx");
  duck_prefab->setAnimation("walk");
  sceneResources->models["duck"] = duck_prefab;
  sceneResources->models["duck"]->mesh = duck_prefab;
  sceneResources->models["duck"]->material = sceneResources->materials["toon"];

  AssimpModel* unicorn_prefab = new AssimpModel();
  unicorn_prefab->loadAssimp("assets/animation/withUV/Animation - Unicorn.fbx");
  unicorn_prefab->setAnimation("walk");
  sceneResources->models["unicorn"] = unicorn_prefab;
  sceneResources->models["unicorn"]->mesh = unicorn_prefab;
  sceneResources->models["unicorn"]->material =
      sceneResources->materials["toon"];
#pragma endregion

  ///////////////////////////////////////////////////////
  printf("\nScene: done loading resources!\n");
  ///////////////////////////////////////////////////////

  // Add local game things
  /**
  GameThing* thing_example = new GameThing;
  thing_example->name = "GT_teapot";
  localGameThings.push_back(thing_example);*/

  // Build the scene graph
  node["map"] = new Node("_map");
  node["map"]->model = sceneResources->models["map"];

  node["skybox"] = new Node("_skybox");
  node["skybox"]->model = sceneResources->models["skybox"];
  node["skybox"]->skybox = true;

  // Push scene nodes to root node
  // ("world" node already created in Scene constructor)

  node["world"]->childnodes.push_back(node["skybox"]);

  // node["world"]->childnodes.push_back(node["teapot1"]);
  // node["world"]->childnodes.push_back(node["cubeTest"]);

  node["world"]->childnodes.push_back(node["collision"]);

  node["world"]->childnodes.push_back(node["map"]);

  // createPlayer(-1); // for testing

  // createItemBox(-1, Item::YellowGiftBox);  // for testing
}

void Scene::init(std::map<int, message::LobbyPlayer> players) {
  //loading++;

  init();

  for (auto& [i, p] : players) {
    skins[i] = p.skin;
  }

}