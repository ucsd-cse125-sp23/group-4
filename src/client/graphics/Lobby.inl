#include "Lobby.h"

void Lobby::init(void) {
  // Create a mesh palette
  sceneResources->meshes["player"] =
      new Obj();  // can only be tied to one object? (not a static
                  // resource)
  sceneResources->meshes["player"]->init("assets/model/dev/model-skeleton.obj");

  sceneResources->models["playerRef"] = new Model;
  sceneResources->models["playerRef"]->mesh = sceneResources->meshes["player"];
  // TODO(matthew) copy over mesh too?
  sceneResources->models["playerRef"]->material =
      sceneResources->materials["toon.blue"];

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
  AssimpModel* amRacoon = new AssimpModel();
  if (!amRacoon->loadAssimp("assets/animation/withUV/Animation -Racoon.fbx")) {
    exit(EXIT_FAILURE);
  }
  amRacoon->update(0.0f);
  sceneResources->models["trash panda"] = amRacoon;
  sceneResources->models["trash panda"]->mesh = amRacoon;
  sceneResources->models["trash panda"]->material =
      sceneResources->materials["toon.blue"];
  player_models.push_back(sceneResources->models["trash panda"]);

  AssimpModel* amBee = new AssimpModel();
  if (!amBee->loadAssimp("assets/animation/withUV/Animation -Bee.fbx")) {
    exit(EXIT_FAILURE);
  }
  amBee->update(0.0f);
  sceneResources->models["bee"] = amBee;
  sceneResources->models["bee"]->mesh = amBee;
  sceneResources->models["bee"]->material =
      sceneResources->materials["toon.blue"];
  player_models.push_back(sceneResources->models["bee"]);

  AssimpModel* amAvocado = new AssimpModel();
  if (!amAvocado->loadAssimp("assets/animation/withUV/Animation -Avocado.fbx")) {
    exit(EXIT_FAILURE);
  }
  amAvocado->update(0.0f);
  sceneResources->models["avocado"] = amAvocado;
  sceneResources->models["avocado"]->mesh = amAvocado;
  sceneResources->models["avocado"]->material =
      sceneResources->materials["toon.blue"];
  player_models.push_back(sceneResources->models["avocado"]);

  AssimpModel* amDuck = new AssimpModel();
  if (!amDuck->loadAssimp("assets/animation/withUV/Animation -Duck.fbx")) {
    exit(EXIT_FAILURE);
  }
  amDuck->update(0.0f);
  sceneResources->models["duck"] = amDuck;
  sceneResources->models["duck"]->mesh = amDuck;
  sceneResources->models["duck"]->material =
      sceneResources->materials["toon.blue"];
  player_models.push_back(sceneResources->models["duck"]);

  AssimpModel* amCat = new AssimpModel();
  if (!amCat->loadAssimp("assets/animation/withUV/Animation -Cat.fbx")) {
    exit(EXIT_FAILURE);
  }
  amCat->update(0.0f);
  sceneResources->models["cat"] = amCat;
  sceneResources->models["cat"]->mesh = amCat;
  sceneResources->models["cat"]->material =
      sceneResources->materials["toon.blue"];
  player_models.push_back(sceneResources->models["cat"]);

  AssimpModel* amUnicorn = new AssimpModel();
  if (!amUnicorn->loadAssimp("assets/animation/withUV/Animation -Unicorn.fbx")) {
    exit(EXIT_FAILURE);
  }
  amUnicorn->update(0.0f);
  sceneResources->models["unicorn"] = amUnicorn;
  sceneResources->models["unicorn"]->mesh = amUnicorn;
  sceneResources->models["unicorn"]->material =
      sceneResources->materials["toon.blue"];
  player_models.push_back(sceneResources->models["unicorn"]);


  ///////////////////////////////////////////////////////
  printf("\nLobby: done loading resources!\n");
  ///////////////////////////////////////////////////////

  GameThing* trash_panda = new GameThing;
  trash_panda->name = "trash panda";
  models.push_back(trash_panda);

  GameThing* bee = new GameThing;
  bee->name = "bee";
  models.push_back(bee);

  GameThing* avocado = new GameThing;
  avocado->name = "avocado";
  models.push_back(avocado);

  GameThing* duck = new GameThing;
  duck->name = "duck";
  models.push_back(duck);

  GameThing* cat = new GameThing;
  cat->name = "cat";
  models.push_back(cat);

  GameThing* unicorn = new GameThing;
  unicorn->name = "unicorn";
  models.push_back(unicorn);

  // Build the scene graph
  node["trash panda"] = trash_panda;
  node["bee"] = bee;
  node["avocado"] = avocado;
  node["duck"] = duck;
  node["cat"] = cat;
  node["unicorn"] = unicorn;

  trash_panda->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  trash_panda->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  trash_panda->model = sceneResources->models["trash_panda"];

  bee->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  bee->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  bee->model = sceneResources->models["bee"];

  avocado->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  avocado->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  avocado->model = sceneResources->models["avocado"];

  duck->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  duck->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  duck->model = sceneResources->models["duck"];

  cat->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  cat->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  cat->model = sceneResources->models["cat"];

  unicorn->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  unicorn->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  unicorn->model = sceneResources->models["unicorn"];

  buildSceneTree();
}
