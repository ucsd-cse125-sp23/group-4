////////////////////////////////////////
// Skin.h (deprecated)
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include "Skeleton.h"
#include "Tokenizer.h"
#include "core.h"
using namespace ::glm;

////////////////////////////////////////////////////////////////////////////////

// The Skin class for storing skinning mesh data.

class Skin {
 public:
  Skin() {
    color = glm::vec3(0.9f, 0.95f, 0.9f);

    // GL
    // Generate VBOs and VAO.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);
  }
  ~Skin() {
    // GL
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
  }

  bool Load(const char* file);
  void Update(Skeleton* skel);
  void Draw(const glm::mat4& viewProjMtx, GLuint shader);

 private:
  GLuint VAO;
  GLuint VBO_positions, VBO_normals, EBO;

  // visuals
  glm::vec3 color;

  // Mesh information
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;  // triangles

  std::vector<std::vector<std::pair<int, float>>> skinweights;  // list of lists

  // init data
  std::vector<glm::vec3> initpositions;
  std::vector<glm::vec3> initnormals;

  // Triangle/Vertex not used

  std::vector<glm::mat4> bindings;
};

////////////////////////////////////////////////////////////////////////////////
