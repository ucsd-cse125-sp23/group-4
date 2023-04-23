#pragma once

#include "AssimpMesh.h"

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

class AssimpNode {
public:
    static unsigned int count;
    static std::map<aiNode*,AssimpNode*> nodeMap;

    AssimpNode();
    void loadFromAssimp(aiNode *node, const aiScene *scene);
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
private:
    const unsigned int id;
    bool isLoaded;

    std::string name;
    std::vector<AssimpMesh*> meshes;
    glm::mat4 localTransform;

    AssimpNode* parent;
    std::vector<AssimpNode*> children;
};