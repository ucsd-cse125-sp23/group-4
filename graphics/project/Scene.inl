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
    sceneResources = new SceneResourceMap();

    // Create a mesh palette
    //sceneResources->meshes["cube"] = new Cube();
    sceneResources->meshes["teapot"] = new Obj();
    sceneResources->meshes["bunny"] = new Obj();

    //sceneResources->meshes["cube"]->init();
    sceneResources->meshes["teapot"]->init("models/teapot.obj");
    sceneResources->meshes["bunny"]->init("models/bunny.obj");

    // Create a shader program with a vertex shader and a fragment shader.
    sceneResources->shaderPrograms["basic"] = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Create a material palette
    sceneResources->materials["wood"] = new Material;
    sceneResources->materials["wood"]->shader = sceneResources->shaderPrograms["basic"];
    sceneResources->materials["wood"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    sceneResources->materials["wood"]->diffuse = vec4(0.3f, 0.15f, 0.1f, 1.0f);
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

    ///////////////////////////////////////////////////////

    // Build the scene graph
    node["teapot1"] = new Node;
    node["teapot2"] = new Node;
    node["bunny"] = new Node;

    node["teapot1"]->transformMtx = translate(vec3(2.0f, 0.0f, 0.0f));
    node["teapot1"]->model = sceneResources->models["teapot1"];

    node["teapot2"]->transformMtx = translate(vec3(0.0f, 2.0f, 0.0f));
    node["teapot2"]->model = sceneResources->models["teapot2"];

    node["bunny"]->transformMtx = translate(vec3(-4.0f, 0.0f, 0.0f));
    node["bunny"]->model = sceneResources->models["bunny1"];

    // "world" node already exists
    node["world"]->childnodes.push_back(node["teapot1"]);
    node["teapot1"]->childnodes.push_back(node["teapot2"]);
    node["world"]->childnodes.push_back(node["bunny"]);

    // Put a camera
    camera = new Camera;
}
