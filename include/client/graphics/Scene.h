/**************************************************

*****************************************************/
#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>
#include <math.h>

#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <stack>

#include "client/graphics/Node.h"
#include "client/graphics/GameThing.h"

#include "client/graphics/Camera.h"
//#include "Light.h"
#include "client/graphics/Mesh.h"
#include "client/graphics/SkinnedMesh.h"
#include "client/graphics/shader.h"
#include "client/graphics/Material.h"
#include "client/graphics/Model.h"
#include "client/graphics/Obj.h"
#include "client/graphics/Cube.h"
#include "client/graphics/Collider.h"

class SceneResourceMap {
public:
    // The following are containers of object pointers serving as "prefabs" to be referenced across the project.
    std::map< string, Mesh* > meshes;
    std::map< string, GLuint > shaderPrograms;
    std::map<string, Material*> materials;
    std::map< string, Model* > models;      // more complex; meshes + other info combined
    //std::map< std::string, Light* > light;

    SceneResourceMap() {
        
    }

    ~SceneResourceMap() {
        // The containers of pointers own the object pointed to by the pointers.
        // All the objects should be deleted when the object palette ("prefab" list) is destructed.
        // mesh geometry
        for (auto entry : meshes) {
            delete entry.second;
        }
        for (auto entry : shaderPrograms) {
            glDeleteProgram(entry.second);
        }
        // materials
        for (auto entry : materials) {
            delete entry.second;
        }
        // models
        for (auto entry : models) {
            delete entry.second;
        }
    }
};

class Scene {
public:
    static bool _freecam;
    static bool _gizmos;
    static SceneResourceMap _globalSceneResources;

    SceneResourceMap* sceneResources;

    Camera* camera; // TODO reimplement this
    
    

    // The container of nodes will be the scene graph after we connect the nodes by setting the child_nodes.
    std::map< std::string, Node* > node;

    std::vector< GameThing* > gamethings;

    Scene() {
        camera = new Camera;
        sceneResources = new SceneResourceMap();

        // globals --
        _globalSceneResources.meshes["_gz-cube"] = new Cube();

        _globalSceneResources.meshes["_gz-xyz"] =
            new Obj();  // gizmo for debugging
        _globalSceneResources.meshes["_gz-xyz"]->init(
            "assets/models/_gizmo.obj");

        _globalSceneResources.shaderPrograms["unlit"] = LoadShaders(
            "assets/shaders/unlit.vert",
                                              "assets/shaders/unlit.frag");

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
        _globalSceneResources.models["_gz-xyz"]->transformMtx =
            glm::scale(glm::vec3(1.0f));

        _globalSceneResources.models["_gz-cube"] = new Model;
        _globalSceneResources.models["_gz-cube"]->mesh =
            _globalSceneResources.meshes["_gz-cube"];
        _globalSceneResources.models["_gz-cube"]->material =
            _globalSceneResources.materials["unlit"];
        _globalSceneResources.models["_gz-cube"]->transformMtx =
            glm::translate(glm::vec3(0.0f));
        // --


        // the default scene graph already has one node named "world."
        node["world"] = new Node("world");
    }

    void init(void);
    void update(float);
    void draw(const glm::mat4& viewProjMtx);

    void gui();

    // destructor
    ~Scene() {
        
        // nodes
        for (std::pair<std::string, Node*> entry : node) {
            delete entry.second;
        }

        delete sceneResources;
        delete camera;
    }
};
