#include "Lobby.h"

void Lobby::init(void) {
  // Create a mesh palette
  sceneResources->meshes["player"] =
      new Obj();  // can only be tied to one object? (not a static
                  // resource)
  sceneResources->meshes["player"]->init("assets/model/dev/model-skeleton.obj");

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
  sceneResources->models["player"] = new Model;
  sceneResources->models["player"]->mesh = sceneResources->meshes["player"];
  sceneResources->models["player"]->material =
      sceneResources->materials["toon.blue"];
  player_models.push_back(sceneResources->models["player"]);

  sceneResources->models["player2"] = new Model;
  sceneResources->models["player2"]->mesh = sceneResources->meshes["player"];
  sceneResources->models["player2"]->material =
      sceneResources->materials["marble"];
  player_models.push_back(sceneResources->models["player2"]);

  sceneResources->models["player3"] = new Model;
  sceneResources->models["player3"]->mesh = sceneResources->meshes["player"];
  sceneResources->models["player3"]->material =
      sceneResources->materials["ceramic"];
  player_models.push_back(sceneResources->models["player3"]);

  sceneResources->models["player4"] = new Model;
  sceneResources->models["player4"]->mesh = sceneResources->meshes["player"];
  sceneResources->models["player4"]->material =
      sceneResources->materials["silver"];
  player_models.push_back(sceneResources->models["player4"]);

  ///////////////////////////////////////////////////////
  printf("\nLobby: done loading resources!\n");
  ///////////////////////////////////////////////////////

  GameThing* thing_player = new GameThing;
  thing_player->name = "player1";
  models.push_back(thing_player);

  GameThing* thing_player2 = new GameThing;
  thing_player2->name = "player2";
  models.push_back(thing_player2);

  GameThing* thing_player3 = new GameThing;
  thing_player3->name = "player3";
  models.push_back(thing_player3);

  GameThing* thing_player4 = new GameThing;
  thing_player4->name = "player4";
  models.push_back(thing_player4);

  // Build the scene graph
  node["player1"] = thing_player;
  node["player2"] = thing_player2;
  node["player3"] = thing_player3;
  node["player4"] = thing_player4;

  thing_player->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  thing_player->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  thing_player->model = sceneResources->models["player"];

  thing_player2->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  thing_player2->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  thing_player2->model = sceneResources->models["player2"];

  thing_player3->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  thing_player3->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  thing_player3->model = sceneResources->models["player3"];

  thing_player4->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  thing_player4->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  thing_player4->model = sceneResources->models["player4"];

  buildSceneTree();
}
