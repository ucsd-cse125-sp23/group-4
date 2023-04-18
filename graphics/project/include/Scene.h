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

#include "Camera.h"
//#include "Light.h"
#include "Mesh.h"
#include "shader.h"
#include "Material.h"
#include "Model.h"

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

    Scene() {
        // the default scene graph already has one node named "world."
        node["world"] = new Node;
    }

    void init(void);
    void update(void);  // TODO: use a separate map of nodes for updateables? "Updated" interface?
    void draw(const glm::mat4& viewProjMtx);

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
