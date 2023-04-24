/**************************************************

*****************************************************/
#define GLM_FORCE_RADIANS
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
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
#include "Player.h"

#include "Camera.h"
//#include "Light.h"
#include "Mesh.h"
#include "SkinnedMesh.h"
#include "shader.h"
#include "Material.h"
#include "Model.h"
#include "PlayerModel.h"

#ifndef __SCENE_H__
#define __SCENE_H__

class SceneResourceMap {
public:
    // The following are containers of object pointers serving as "prefabs" to be referenced across the project.
    std::map< std::string, Skeleton*> skeletons;
    std::map< std::string, Mesh* > meshes;
    std::map< std::string, std::map<std::string, AnimationPlayer*>> animations;
    std::map< std::string, GLuint > shaderPrograms;
    std::map< std::string, Material* > materials;
    std::map< std::string, Model* > models;      // more complex; meshes + other info combined
    //std::map< std::string, Light* > light;

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
    void update(GLFWwindow* window, Camera* camera, float delta, float step = 0.25);
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
