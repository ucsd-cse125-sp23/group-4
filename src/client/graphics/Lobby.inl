#include "client/graphics/Lobby.h"

void Lobby::init(void) {
  sceneResources->meshes["cube"] = new Cube();
  sceneResources->meshes["teapot"] = new Obj();
  sceneResources->meshes["player"] = new Obj();

  sceneResources->meshes["wasp"] =
      new SkinnedMesh();  // can only be tied to one object? (not a static
                          // resource)
  sceneResources->meshes["wasp"]->init("assets/models/wasp.skin");

  sceneResources->meshes["teapot"]->init("assets/models/teapot.obj");
  sceneResources->meshes["player"]->init("assets/models/player_v1.obj");

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
      LoadShaders("assets/shaders/shader.vert", "assets/shaders/shader.frag");
  sceneResources->shaderPrograms["basicTex"] =
      LoadShaders("assets/shaders/shaderx.vert", "assets/shaders/shaderx.frag");

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
  sceneResources->materials["ceramic"]->shininess = 150.0f;

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
      sceneResources->shaderPrograms["basicTex"];
  sceneResources->materials["grid"]->texture = sceneResources->textures["grid"];
  sceneResources->materials["grid"]->diffuse = vec4(0.7f, 0.7f, 0.7f, 1.0f);

  // Create a model palette
  sceneResources->models["teapot1"] = new Model;
  sceneResources->models["teapot1"]->mesh = sceneResources->meshes["teapot"];
  sceneResources->models["teapot1"]->material =
      sceneResources->materials["silver"];
  sceneResources->models["teapot1"]->modelMtx = scale(vec3(1.5f));
 
  
  sceneResources->models["player"] = new Model;
  sceneResources->models["player"]->mesh = sceneResources->meshes["player"];
  sceneResources->models["player"]->material =
      sceneResources->materials["ceramic"];
  sceneResources->models["cubeTextured"] = new Model;
  sceneResources->models["cubeTextured"]->mesh =
      sceneResources->meshes["cubeUV"];
  sceneResources->models["cubeTextured"]->material =
      sceneResources->materials["grid"];

  PlayerModel* waspModel = new PlayerModel;
  waspModel->skel = sceneResources->skeletons["wasp"];
  waspModel->skin = dynamic_cast<SkinnedMesh*>(sceneResources->meshes["wasp"]);
  waspModel->anims = sceneResources->animations["wasp"];

  sceneResources->models["wasp"] = waspModel;
  sceneResources->models["wasp"]->mesh = sceneResources->meshes["wasp"];
  sceneResources->models["wasp"]->material = sceneResources->materials["wood"];
  player_models.push_back(waspModel);

  PlayerModel* waspModel2 = new PlayerModel;
  waspModel2->skel = sceneResources->skeletons["wasp"];
  waspModel2->skin = dynamic_cast<SkinnedMesh*>(sceneResources->meshes["wasp"]);
  waspModel2->anims = sceneResources->animations["wasp"];

  sceneResources->models["wasp2"] = waspModel2;
  sceneResources->models["wasp2"]->mesh = sceneResources->meshes["wasp"];
  sceneResources->models["wasp2"]->material =
      sceneResources->materials["marble"];
  player_models.push_back(waspModel2);

  PlayerModel* waspModel3 = new PlayerModel;
  waspModel3->skel = sceneResources->skeletons["wasp"];
  waspModel3->skin = dynamic_cast<SkinnedMesh*>(sceneResources->meshes["wasp"]);
  waspModel3->anims = sceneResources->animations["wasp"];

  sceneResources->models["wasp3"] = waspModel3;
  sceneResources->models["wasp3"]->mesh = sceneResources->meshes["wasp"];
  sceneResources->models["wasp3"]->material =
      sceneResources->materials["ceramic"];
  player_models.push_back(waspModel3);

  PlayerModel* waspModel4 = new PlayerModel;
  waspModel4->skel = sceneResources->skeletons["wasp"];
  waspModel4->skin = dynamic_cast<SkinnedMesh*>(sceneResources->meshes["wasp"]);
  waspModel4->anims = sceneResources->animations["wasp"];

  sceneResources->models["wasp4"] = waspModel4;
  sceneResources->models["wasp4"]->mesh = sceneResources->meshes["wasp"];
  sceneResources->models["wasp4"]->material =
      sceneResources->materials["silver"];
    player_models.push_back(waspModel4);

  ///////////////////////////////////////////////////////
  printf("\nScene: done loading resources!\n");
  ///////////////////////////////////////////////////////

  // Add stuff to game updateables
  GameThing* thing_wasp = new GameThing;
  thing_wasp->name = "wasp1";
  gamethings.push_back(thing_wasp);

  GameThing* thing_wasp2 = new GameThing;
  thing_wasp2->name = "wasp2";
  gamethings.push_back(thing_wasp2);

  GameThing* thing_wasp3 = new GameThing;
  thing_wasp3->name = "wasp3";
  gamethings.push_back(thing_wasp3);

  GameThing* thing_wasp4 = new GameThing;
  thing_wasp4->name = "wasp4";
  gamethings.push_back(thing_wasp4);

  // Build the scene graph
  node["wasp1"] = thing_wasp;
  node["wasp2"] = thing_wasp2;
  node["wasp3"] = thing_wasp3;
  node["wasp4"] = thing_wasp4;

  thing_wasp->transform.position = vec3(0.0f, 0.0f, 0.0f);
  thing_wasp->model = sceneResources->models["wasp"];

  thing_wasp2->transform.position = vec3(0.0f, 0.0f, 0.0f);
  thing_wasp2->model = sceneResources->models["wasp2"];

  thing_wasp3->transform.position = vec3(0.0f, 0.0f, 0.0f);
  thing_wasp3->model = sceneResources->models["wasp3"];

  thing_wasp4->transform.position = vec3(0.0f, 0.0f, 0.0f);
  thing_wasp4->model = sceneResources->models["wasp4"];

  buildSceneTree();
}
