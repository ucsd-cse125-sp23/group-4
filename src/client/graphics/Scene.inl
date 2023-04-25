/**************************************************
Scene.inl contains the definition of the scene graph
*****************************************************/
#include "Scene.h"
#include "Cube.h"
#include "Obj.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;
void Scene::init(void) {
    // Create a mesh palette
    sceneResources->meshes["cube"] = new Cube();
    sceneResources->meshes["teapot"] = new Obj();
    sceneResources->meshes["bunny"] = new Obj();

    sceneResources->meshes["wasp"] = new SkinnedMesh(); // can only be tied to one object? (not a static resource)
    sceneResources->meshes["wasp"]->init("assets/models/wasp.skin");

    sceneResources->meshes["teapot"]->init("assets/models/teapot.obj");
    sceneResources->meshes["bunny"]->init("assets/models/bunny.obj");

    sceneResources->skeletons["wasp"] = new Skeleton();
    sceneResources->skeletons["wasp"]->Load("assets/models/wasp.skel");

    AnimationClip* animClip = new AnimationClip();
    animClip->Load("assets/models/wasp_walk.anim");
    sceneResources->animations["wasp"]["walk"] = new AnimationPlayer();
    sceneResources->animations["wasp"]["walk"]->SetClip(animClip);
    
    // Create a shader program with a vertex shader and a fragment shader.
    sceneResources->shaderPrograms["basic"] = LoadShaders("assets/shaders/shader.vert", "assets/shaders/shader.frag");

    // Create a material palette
    sceneResources->materials["wood"] = new Material;
    sceneResources->materials["wood"]->shader = sceneResources->shaderPrograms["basic"];
    sceneResources->materials["wood"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    sceneResources->materials["wood"]->diffuse = vec4(0.4f, 0.15f, 0.1f, 1.0f);
    sceneResources->materials["wood"]->specular = vec4(0.3f, 0.15f, 0.1f, 1.0f);
    sceneResources->materials["wood"]->shininess = 100.0f;

    sceneResources->materials["ceramic"] = new Material;
    sceneResources->materials["ceramic"]->shader = sceneResources->shaderPrograms["basic"];
    sceneResources->materials["ceramic"]->ambient = vec4(0.02f, 0.07f, 0.2f, 1.0f);
    sceneResources->materials["ceramic"]->diffuse = vec4(0.1f, 0.25f, 0.7f, 1.0f);
    sceneResources->materials["ceramic"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    sceneResources->materials["ceramic"]->shininess = 150.0f;

    sceneResources->materials["silver"] = new Material;
    sceneResources->materials["silver"]->shader = sceneResources->shaderPrograms["basic"];
    sceneResources->materials["silver"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    sceneResources->materials["silver"]->diffuse = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    sceneResources->materials["silver"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    sceneResources->materials["silver"]->shininess = 50.0f;

    sceneResources->materials["marble"] = new Material;
    sceneResources->materials["marble"]->shader = sceneResources->shaderPrograms["basic"];
    sceneResources->materials["marble"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    sceneResources->materials["marble"]->diffuse = vec4(0.97f, 0.89f, 0.75f, 1.0f);
    sceneResources->materials["marble"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    sceneResources->materials["marble"]->shininess = 50.0f;

    // Create a model palette
    sceneResources->models["teapot1"] = new Model;
    sceneResources->models["teapot1"]->mesh = sceneResources->meshes["teapot"];
    sceneResources->models["teapot1"]->material = sceneResources->materials["silver"];
    sceneResources->models["teapot1"]->transformMtx = scale(vec3(1.5f));
    sceneResources->models["teapot2"] = new Model;
    sceneResources->models["teapot2"]->mesh = sceneResources->meshes["teapot"];
    sceneResources->models["teapot2"]->material = sceneResources->materials["ceramic"];
    sceneResources->models["teapot2"]->transformMtx = scale(vec3(1.0f, 1.5f, 1.0f)) * scale(vec3(0.5f));
    sceneResources->models["bunny1"] = new Model;
    sceneResources->models["bunny1"]->mesh = sceneResources->meshes["bunny"];
    sceneResources->models["bunny1"]->material = sceneResources->materials["wood"];
    sceneResources->models["bunny1"]->transformMtx = rotate(60.0f, vec3(0.0f, 1.0f, 0.0f));
    sceneResources->models["cube1"] = new Model;
    sceneResources->models["cube1"]->mesh = sceneResources->meshes["cube"];
    sceneResources->models["cube1"]->material = sceneResources->materials["silver"];
    sceneResources->models["cubeF"] = new Model;
    sceneResources->models["cubeF"]->transformMtx = scale(vec3(1.0f, 0.1f, 1.0f)) * scale(vec3(0.5f));
    sceneResources->models["cubeF"]->mesh = sceneResources->meshes["cube"];
    sceneResources->models["cubeF"]->material = sceneResources->materials["marble"];

    PlayerModel* waspModel = new PlayerModel;
    waspModel->skel = sceneResources->skeletons["wasp"];
    waspModel->skin = dynamic_cast<SkinnedMesh*>(sceneResources->meshes["wasp"]);
    waspModel->anims = sceneResources->animations["wasp"];

    sceneResources->models["wasp"] = waspModel;
    sceneResources->models["wasp"]->mesh = sceneResources->meshes["wasp"];
    sceneResources->models["wasp"]->material = sceneResources->materials["wood"];

    ///////////////////////////////////////////////////////

    // Add stuff to game updateables
    GameThing* thing_example = new GameThing;
    thing_example->name = "GT_teapot";
    gamethings.push_back(thing_example);

    
    Player* player = new Player();
    player->mod = waspModel;
    player->name = "Player 1";
    gamethings.push_back(player);

    // Build the scene graph
    node["teapot1"] = thing_example;
    node["teapot2"] = new Node("teapotChild");
    node["bunny"] = new Node("bunny");
    node["ground"] = new Node("ground");
    node["wasp"] = player;

    thing_example->transform.position = vec3(2.0f, 0.0f, 0.0f); // gamething only
    node["teapot1"]->model = sceneResources->models["teapot1"];

    node["teapot2"]->transformMtx = translate(vec3(0.0f, 1.0f, 0.0f));
    node["teapot2"]->model = sceneResources->models["teapot2"];

    node["bunny"]->transformMtx = translate(vec3(-4.0f, 0.0f, 0.0f));
    node["bunny"]->model = sceneResources->models["bunny1"];

    node["wasp"]->model = sceneResources->models["wasp"];

    node["ground"]->transformMtx = translate(vec3(-10, -1.0f, -10));
    int checkers = 16;
    for (int i = 1; i <= checkers; i++) {
        for (int j = 1; j <= checkers; j++) {
            if ((i + j) % 2 == 0) {
                std::string currcube = "cube." + std::to_string(i) + ", " + std::to_string(j);

                node[currcube] = new Node(currcube);
                node[currcube]->transformMtx = translate(vec3(i, 0, j));
                node[currcube]->model = sceneResources->models["cubeF"];

                node["ground"]->childnodes.push_back(node[currcube]);
            }
        }
    }

    // "world" node already exists
    node["world"]->childnodes.push_back(node["teapot1"]);
    node["teapot1"]->childnodes.push_back(node["teapot2"]);
    node["world"]->childnodes.push_back(node["bunny"]);
    node["world"]->childnodes.push_back(node["ground"]);
    node["world"]->childnodes.push_back(node["wasp"]);


}
