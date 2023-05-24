#pragma once

#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "client/graphics/AssimpNode.h"
#include "client/graphics/core.h"

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
    glm::vec2  uv          = glm::vec2(0,0);
};

/** The AssimpMesh class for storing a mesh with a binded skeleton. */
class AssimpMesh {
 public:
  static const int MAX_BONES = 200;
  AssimpMesh();

    std::string name;
    AssimpNode* node;
    /** vertex relative to world space (default pose) */
    std::vector<Vertex>       vertices;
    /** vertex relative to world space (current pose) */
    std::vector<Vertex>       worldVerticies;
    std::vector<unsigned int> indices;
    std::vector<AssimpJoint*> joints;
    glm::mat4 matBindingInvs[MAX_BONES];

    void update();
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void gl_load();
    void gl_delete();
    void imGui();

    void draw();
    void gl_load2();
    void gl_update2();

   private:
    unsigned int VAO, VBO, EBO;
    unsigned int VAO2, VBO2, EBO2;
    bool isLoaded, isLoaded2;
};
