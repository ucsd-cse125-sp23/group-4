/**************************************************
 * SkinnedMesh.h
 * implementation of Mesh that also supports a skeleton
 *****************************************************/

#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <vector>

#include "Mesh.h"
#include "Skeleton.h"

////////////////////////////////////////////////////////////////////////////////

// The Skin class for storing skinning mesh data.

class SkinnedMesh : public Mesh {
 public:
  SkinnedMesh() { creategl(); }
  ~SkinnedMesh() { cleargl(); }

  void init(const char* filename);

  void updateSkin(Skeleton* skel);

  void draw(void) {
    // special skin behavior: always rebind vertices(?)
    bindgl(vertices_curr, normals, indices);

    Mesh::draw();
  }

 private:
  bool LoadCustom(const char* file);  // from 169 project
  bool LoadFBX(const char* file);     // TODO: assimp

  // Mesh information of skin (updated based on skeleton)
  std::vector<glm::vec3> vertices_bind;  // init data (bind pose)
  std::vector<glm::vec3> vertices_curr;  // updated positions
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;

  // ASSIMP?
  /*
          std::vector<glm::ivec4> boneIds;
          std::vector<glm::vec4> weights;
  */

  std::vector<std::vector<std::pair<int, float>>> skinweights;  // list of lists

  std::vector<glm::mat4> bindings;  // binding weight matrices for bones(?)
};

////////////////////////////////////////////////////////////////////////////////
