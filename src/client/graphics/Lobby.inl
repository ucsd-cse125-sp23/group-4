#include <config/lib.hpp>

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
  auto config = get_config();

  for (auto& skName : skin_names) {
    AssimpModel* am = new AssimpModel();
    std::string path =
        std::string(config["skin_dir"]) + std::string(config["skin_" + skName]);
    if (!am->loadAssimp(path.c_str())) {
      exit(EXIT_FAILURE);
    }

    sceneResources->models[skName] = am;
    sceneResources->models[skName]->mesh = am;
    sceneResources->models[skName]->material =
        sceneResources->materials["toon"];
    player_models.push_back(sceneResources->models[skName]);

    GameThing* skThing = new GameThing;
    skThing->name = skName;
    models.push_back(skThing);

    node[skName] = skThing;
    skThing->transform.position = glm::vec3(0.0f, -3.0f, 0.0f);
    skThing->transform.rotation = glm::vec3(-10.0f, 180.0f, 0.0f);
    skThing->model = sceneResources->models[skName];
  }

  ///////////////////////////////////////////////////////
  printf("\nLobby: done loading resources!\n");
  ///////////////////////////////////////////////////////
  for (auto m : models) {
    m->update(0.0f);
  }

  thing_player->transform.position = glm::vec3(0.0f, -1.5f, 0.0f);
  thing_player->transform.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
  thing_player->model = sceneResources->models["player"];

  background.init("assets/UI/Group 8.png");
  highlight.init("assets/UI/Vector 15 (1).png");
  point.init("assets/UI/Polygon 6 (1).png");

  ready_icons["trash panda"].init("assets/UI/IMG_2421.PNG");
  icons["trash panda"].init("assets/UI/IMG_2422.PNG");
  icons["bee"].init("assets/UI/IMG_2423.PNG");
  ready_icons["bee"].init("assets/UI/IMG_2424.PNG");
  icons["avocado"].init("assets/UI/IMG_2425.PNG");
  ready_icons["avocado"].init("assets/UI/IMG_2426.PNG");
  icons["duck"].init("assets/UI/IMG_2427.PNG");
  ready_icons["duck"].init("assets/UI/IMG_2428.PNG");
  icons["cat"].init("assets/UI/IMG_2429.PNG");
  ready_icons["cat"].init("assets/UI/IMG_2430.PNG");
  icons["unicorn"].init("assets/UI/IMG_2431.PNG");
  ready_icons["unicorn"].init("assets/UI/IMG_2432.PNG");
  icons["waffle"].init("assets/UI/IMG_2436.PNG");
  ready_icons["waffle"].init("assets/UI/IMG_2437.PNG");
  icons["bear"].init("assets/UI/IMG_2491.PNG");
  ready_icons["bear"].init("assets/UI/IMG_2492.PNG");
}
