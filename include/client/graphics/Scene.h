/**************************************************

*****************************************************/
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

#include "Node.h"
#include "GameThing.h"

#include "Camera.h"
//#include "Light.h"
#include "Mesh.h"
#include "SkinnedMesh.h"
#include "shader.h"
#include "Material.h"
#include "Model.h"
#include "Obj.h"
#include "Cube.h"

#ifndef __SCENE_H__
#define __SCENE_H__

using namespace std;

class SceneResourceMap {
public:
    // The following are containers of object pointers serving as "prefabs" to be referenced across the project.
    map< string, Mesh* > meshes;
    map< string, GLuint > shaderPrograms;
    map< string, Material* > materials;
    map< string, Model* > models;      // more complex; meshes + other info combined
    //std::map< std::string, Light* > light;

    SceneResourceMap() {
        meshes["_gz-cube"] = new Cube();

        meshes["_gz-xyz"] = new Obj();  // gizmo for debugging
        meshes["_gz-xyz"]->init("assets/models/_gizmo.obj");

        shaderPrograms["unlit"] = LoadShaders(
            "assets/shaders/unlit.vert", "assets/shaders/unlit.frag");

        materials["unlit"] = new Material;
        materials["unlit"]->shader = shaderPrograms["unlit"];
        materials["unlit"]->diffuse =
            glm::vec4(0.99f, 0.0f, 0.86f, 1.0f);

        models["_gz-xyz"] = new Model;
        models["_gz-xyz"]->mesh = meshes["_gz-xyz"];
        models["_gz-xyz"]->material = materials["unlit"];
        models["_gz-xyz"]->transformMtx = glm::scale(glm::vec3(1.0f));
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
    bool _freecam = false;
    bool _gizmos = false;

    Camera* camera;
    
    SceneResourceMap* sceneResources;

    // The container of nodes will be the scene graph after we connect the nodes by setting the child_nodes.
    std::map< std::string, Node* > node;

    std::vector< GameThing* > gamethings;

    Scene() {
        camera = new Camera;
        sceneResources = new SceneResourceMap();

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

#endif 
