#pragma once

#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>

#include "client/graphics/AssimpMesh.h"
#include "client/graphics/Cube.h"

class AssimpMesh;
struct AssimpJoint;

/** The AssimpNode class for a local representation of a scene node. */
class AssimpNode {
 public:
  AssimpNode(unsigned int id);

  /** Numeric identifier that is unique for nodes in an AssimpModel */
  const unsigned int id;
  std::string name;
  std::vector<AssimpMesh*> meshes;
  /** transformation matrix relative to parent node (default pose) */
  glm::mat4 localTransform;
  /** transformation matrix relative to world space (default pose) */
  glm::mat4 accTransform;
  /** transformation matrix relative to parent node (current pose) */
  glm::mat4 animationTransform;
  /** transformation matrix relative to world space (current pose) */
  glm::mat4 matWorldTransform;

  AssimpNode* parent;
  std::vector<AssimpNode*> children;
  std::vector<AssimpJoint*> joints;

  bool isMarked = false;

  void update(const glm::mat4& parentWorldTransform);
  void draw(const glm::mat4& viewProjMtx);
  void imGui();

 private:
  static GLuint* shader;
  Cube geometry = Cube(glm::vec3(-0.1, -0.1, -0.1), glm::vec3(0.1, 0.1, 0.1));
};
