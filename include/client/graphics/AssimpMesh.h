#pragma once

#include "core.h"
#include "AssimpNode.h"

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class AssimpNode;

struct VertexWeight {
    unsigned int vertexInd;
    float weight;
};

struct AssimpJoint {
    unsigned int ind;
    std::string name;
    glm::mat4 matBindingInv;
    glm::mat4 matWorldTransform;
    std::vector<VertexWeight> weights;
    AssimpNode *node, *meshNode;

    glm::mat4 update();
};

struct Vertex {
    glm::vec3  position;
    glm::vec3  normal;
    glm::uvec4 boneInds    = glm::vec4(0,0,0,0);
    glm::vec4  boneWeights = glm::vec4(1,0,0,0);
};

class AssimpMesh {
public:
    AssimpMesh();

    AssimpNode* node;
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<AssimpJoint*> joints;
    glm::mat4 matBindingInvs[200];

    void update();
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void gl_load();
    void gl_delete();
    void imGui();
private:
    unsigned int VAO, VBO, EBO;
    bool isLoaded, isDrawMesh;
};
