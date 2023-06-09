#pragma once

#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "client/graphics/AssimpNode.h"
#include "client/graphics/Texture.h"
#include "client/graphics/core.h"

class AssimpNode;
class AssimpMesh;

struct VertexWeight {
  unsigned int vertexInd;
  float weight;
};

struct NodeJoint {
  NodeJoint(AssimpMesh* const mesh, const glm::mat4 invBindMat)
      : mesh(mesh), invBindMat(invBindMat) {}

  /** apply the contribution of this joint to its related mesh */
  glm::mat4 update();

  /** the mesh that this joint relates to */
  AssimpMesh* const mesh;
  /** transform from mesh space to joint space */
  const glm::mat4 invBindMat;
  /** the weights of each vertex affected by this joint */
  std::vector<VertexWeight> vertexWeights;
};

/** The AssimpMesh class for storing a mesh with a binded skeleton. */
class AssimpMesh {
 public:
  AssimpMesh();
  /** Destructor */
  ~AssimpMesh();

  std::string name;

  // Rendering - Basics
  /** indicates whether vertex data needs to be renewed to GL */
  bool isUpdated = false;
  std::vector<unsigned int> indices;
  std::vector<glm::vec4> outPos;
  std::vector<glm::vec4> outNorm;
  std::vector<glm::vec2> uvs;
  /** vertex relative to world space (default pose) */
  std::vector<glm::vec4> pos;
  std::vector<glm::vec4> norm;

  // Rendering - Material & Texture
  /** holds texture of the mesh */
  Texture* texture = nullptr;
  // store them as RGBA colors to avoid conversion,
  //   even though assimp reads RGB colors
  glm::vec4 diffuse, specular, ambient, emissive;
  /** exponent of phong specular equation */
  float shininess;

  void draw();
  void gl_load();
  void gl_update();
  void gl_delete();

 private:
  unsigned int VAO, VBOP, VBON, VBOUV, EBO;
  bool isLoaded;
};
