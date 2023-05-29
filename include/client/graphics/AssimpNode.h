#pragma once

#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>

#include "client/graphics/AssimpMesh.h"
#include "client/graphics/Cube.h"

class AssimpMesh;
struct NodeJoint;

/** The AssimpNode class for a local representation of a scene node. */
class AssimpNode {
 public:
  explicit AssimpNode(unsigned int id);

  void update(const glm::mat4& accWorldMtx);
  void draw(const glm::mat4& viewProjMtx);

  // Hierarchical props
  /** Numeric identifier that is unique for nodes in an AssimpModel */
  const unsigned int id;
  std::string name;
  std::vector<AssimpNode*> children;

  // Rendering props
  std::vector<NodeJoint> joints;
  std::vector<AssimpMesh*> meshes;
  /** transformation matrix relative to parent node (default pose) */
  glm::mat4 localTransform;
  /** transformation matrix relative to parent node (current pose) */
  glm::mat4 animationTransform;
  /** transformation matrix relative to world space (current pose) */
  glm::mat4 matWorldTransform;

  // Debug
  bool isMarked = false;
  void imGui();

 private:
  // Debug
  static GLuint* shader;
  Cube geometry = Cube(glm::vec3(-0.1, -0.1, -0.1), glm::vec3(0.1, 0.1, 0.1));
};
