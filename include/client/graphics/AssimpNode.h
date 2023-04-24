#pragma once

#include "AssimpMesh.h"

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <map>

class AssimpMesh;

class AssimpNode {
public:
    AssimpNode(unsigned int id);

    /** Numeric identifier that is unique for nodes in an AssimpModel */
    const unsigned int id;
    std::string name;
    std::vector<AssimpMesh*> meshes;
    glm::mat4 localTransform;
    glm::mat4 animationTransform;

    AssimpNode* parent;
    std::vector<AssimpNode*> children;

    // void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void imGui();
};
