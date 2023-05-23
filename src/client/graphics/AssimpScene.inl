/**************************************************
 * Scene.inl
 * contains the definition of the scene graph
 *****************************************************/
#include "client/graphics/ColliderImporter.h"
#include "client/graphics/Scene.h"
#include "client/graphics/AssimpModel.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;
void Scene::init(void) {
  AssimpModel* am = new AssimpModel();
  if (!am->loadAssimp(
          "assets/model/Animation -Avocado.fbx")) {
    exit(EXIT_FAILURE);
  }

  // Create a mesh palette
  sceneResources->meshes["cube"] = new Cube();
  sceneResources->meshes["teapot"] = new Obj();
  sceneResources->meshes["bunny"] = new Obj();
  sceneResources->meshes["player"] = new Obj();

  sceneResources->meshes["assimp"] = am;

  sceneResources->meshes["teapot"]->init("assets/models/teapot.obj");
  sceneResources->meshes["bunny"]->init("assets/models/bunny.obj");
  sceneResources->meshes["player"]->init("assets/models/model-skeleton.obj");

  sceneResources->meshes["cubeUV"] = new Obj();
  sceneResources->meshes["cubeUV"]->init(
      "assets/models/bevel_cube.obj");  // includes UV data

  sceneResources->skeletons["wasp"] = new Skeleton();
  sceneResources->skeletons["wasp"]->Load("assets/models/wasp.skel");

  AnimationClip* animClip = new AnimationClip();
  animClip->Load("assets/models/wasp_walk.anim");
  sceneResources->animations["wasp"]["walk"] = new AnimationPlayer();
  sceneResources->animations["wasp"]["walk"]->SetClip(animClip);
  sceneResources->animations["wasp"]["walk"]->speed = 4.0f;  // walk faster

  // Create a shader program with a vertex shader and a fragment shader.
  sceneResources->shaderPrograms["basic"] =
      LoadShaders("assets/shaders/shader.vert", "assets/shaders/shaderx.frag");
  sceneResources->shaderPrograms["toon"] =
      LoadShaders("assets/shaders/shader.vert", "assets/shaders/toon.frag");

  // Create a material palette
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

  sceneResources->textures["grid"] = new Texture;
  sceneResources->textures["grid"]->init("assets/image/test_uv.png");

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


  // Create a model palette
  sceneResources->models["teapot1"] = new Model;
  sceneResources->models["teapot1"]->mesh = sceneResources->meshes["teapot"];
  sceneResources->models["teapot1"]->material =
      sceneResources->materials["toon.blue"];
  sceneResources->models["teapot1"]->modelMtx = scale(vec3(1.5f));
  sceneResources->models["teapot2"] = new Model;
  sceneResources->models["teapot2"]->mesh = sceneResources->meshes["teapot"];
  sceneResources->models["teapot2"]->material =
      sceneResources->materials["ceramic"];
  sceneResources->models["teapot2"]->modelMtx = translate(vec3(0, 0, 0.1f)) *
                                                scale(vec3(1.0f, 1.5f, 1.0f)) *
                                                scale(vec3(0.5f));
  sceneResources->models["bunny1"] = new Model;
  sceneResources->models["bunny1"]->mesh = sceneResources->meshes["bunny"];
  sceneResources->models["bunny1"]->material =
      sceneResources->materials["wood"];
  sceneResources->models["bunny1"]->modelMtx =
      rotate(60.0f, vec3(0.0f, 1.0f, 0.0f));
  sceneResources->models["player"] = new Model;
  sceneResources->models["player"]->mesh = sceneResources->meshes["player"];
  sceneResources->models["player"]->material =
      sceneResources->materials["toon.blue"];
  sceneResources->models["player2"] = new Model;
  sceneResources->models["player2"]->mesh = sceneResources->meshes["player"];
  sceneResources->models["player2"]->material =
      sceneResources->materials["ceramic"];
  sceneResources->models["cube1"] = new Model;
  sceneResources->models["cube1"]->mesh = sceneResources->meshes["cube"];
  sceneResources->models["cube1"]->material =
      sceneResources->materials["silver"];
  sceneResources->models["cubeF"] = new Model;
  sceneResources->models["cubeF"]->modelMtx =
      scale(vec3(1.0f, 0.1f, 1.0f)) * scale(vec3(0.5f));
  sceneResources->models["cubeF"]->mesh = sceneResources->meshes["cube"];
  sceneResources->models["cubeF"]->material =
      sceneResources->materials["marble"];
  sceneResources->models["cubeTextured"] = new Model;
  sceneResources->models["cubeTextured"]->mesh =
      sceneResources->meshes["cubeUV"];
  sceneResources->models["cubeTextured"]->material =
      sceneResources->materials["grid"];

  PlayerModel* assimpModel = am;

  sceneResources->models["assimp"] = assimpModel;
  sceneResources->models["assimp"]->mesh = sceneResources->meshes["assimp"];
  sceneResources->models["assimp"]->material = sceneResources->materials["toon.blue"];

  ///// maps:

  sceneResources->meshes["map1"] = new Obj();
  sceneResources->meshes["map1"]->init("assets/models/map_testing.obj");
  sceneResources->models["map1"] = new Model;
  sceneResources->models["map1"]->mesh = sceneResources->meshes["map1"];
  sceneResources->models["map1"]->material =
      sceneResources->materials["marble"];
  sceneResources->models["map1"]->modelMtx = translate(vec3(0, -2, 0));

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

  std::vector<Collider> mapColliders =
      ColliderImporter::ImportCollisionData("assets/models/test_colliders.obj");

  node["collision"] = new Node("_colliders");
  for (auto c : mapColliders) {
    node["collision"]->childnodes.push_back(new Collider(c));
  }
  // node["collision"]->transformMtx =
  // sceneResources->models["mapColsTesting"]->transformMtx;

  ///////////////////////////////////////////////////////
  printf("\nScene: done loading resources!\n");
  ///////////////////////////////////////////////////////

  // Add stuff to game updateables
  GameThing* thing_example = new GameThing;
  thing_example->name = "GT_teapot";
  gamethings.push_back(thing_example);

  GameThing* thing_cube = new GameThing;
  thing_cube->name = "GT_cube";
  gamethings.push_back(thing_cube);

  GameThing* thing_modeltest = new GameThing;
  thing_modeltest->name = "GT_playerTest";
  gamethings.push_back(thing_modeltest);

  GameThing* thing_modeltestB = new GameThing;
  thing_modeltestB->name = "GT_playerTestB";
  gamethings.push_back(thing_modeltestB);

  Player* player = new Player();
  player->camera = camera;               // give a reference to the game camera
  player->pmodel = assimpModel;            // updating!
  player->model = assimpModel;           // drawing!
  player->pmodel->setAnimation("walk");  // TODO: make this automated
  player->name = "Player 1";
  player->transform.position = vec3(0, 2, 0);
  player->transform.updateMtx(&(player->transformMtx));
  gamethings.push_back(player);

  // Build the scene graph
  node["teapot1"] = thing_example;
  node["teapot2"] = new Node("teapotChild");
  node["bunny"] = new Node("bunny");
  node["cubeT"] = thing_cube;
  node["playerA"] = thing_modeltest;
  node["playerB"] = thing_modeltestB;
  node["ground"] = new Node("ground");
  node["assimp"] = player;

  node["map"] = new Node("_map");
  node["map"]->model = sceneResources->models["map1"];

  node["map2"] = new Node("_map2");
  node["map2"]->model = sceneResources->models["mapColsTesting"];

  thing_example->transform.position = vec3(2.0f, 0.0f, 0.0f);  // gamething only
  node["teapot1"]->model = sceneResources->models["teapot1"];

  thing_cube->transform.position = vec3(2.0f, 3.0f, -4.0f);
  node["cubeT"]->model = sceneResources->models["cubeTextured"];

  thing_modeltest->transform.position = vec3(5.0f, 2.0f, 2.0f);
  thing_modeltest->model = sceneResources->models["player"];
  thing_modeltestB->transform.position = vec3(9.0f, 2.0f, 2.0f);
  thing_modeltestB->model = sceneResources->models["player2"];

  node["teapot2"]->transformMtx = translate(vec3(0.0f, 1.0f, 0.0f));
  node["teapot2"]->model = sceneResources->models["teapot2"];

  node["bunny"]->transformMtx = translate(vec3(-4.0f, 2.0f, 0.0f));
  node["bunny"]->model = sceneResources->models["bunny1"];

  node["assimp"]->model = sceneResources->models["assimp"];

  node["ground"]->transformMtx = translate(vec3(-10, 1.0f, -10));
  int checkers = 16;
  for (int i = 1; i <= checkers; i++) {
    for (int j = 1; j <= checkers; j++) {
      if ((i + j) % 2 == 0) {
        std::string currcube =
            "cube." + std::to_string(i) + ", " + std::to_string(j);

        node[currcube] = new Node(currcube);
        node[currcube]->transformMtx = translate(vec3(i, 0, j));
        node[currcube]->model = sceneResources->models["cubeF"];
        node[currcube]->_renderGizmo = false;

        node["ground"]->childnodes.push_back(node[currcube]);
      }
    }
  }

  player->childnodes.push_back(camera);

  // "world" node already exists
  node["world"]->childnodes.push_back(node["playerA"]);
  node["world"]->childnodes.push_back(node["playerB"]);
  node["world"]->childnodes.push_back(node["teapot1"]);
  node["world"]->childnodes.push_back(node["cubeT"]);
  node["teapot1"]->childnodes.push_back(node["teapot2"]);
  node["world"]->childnodes.push_back(node["bunny"]);
  node["world"]->childnodes.push_back(node["assimp"]);  // test player

  node["world"]->childnodes.push_back(node["ground"]);
  node["world"]->childnodes.push_back(node["collision"]);

  node["world"]->childnodes.push_back(node["map2"]);
}
