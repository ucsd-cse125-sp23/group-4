#pragma once

#include "client/graphics/Mesh.h"
#include "client/graphics/core.h"

class Skybox : public Mesh {
 public:
  Skybox() {
    creategl();

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    // Specify vertex positions
    positions = {glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1),
                 glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1),
                 glm::vec3(-1, 1, 1),  glm::vec3(1, 1, 1),
                 glm::vec3(1, 1, -1),  glm::vec3(-1, 1, -1)};

    // Specify normals (unused)
    normals = {// Front
               glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),
               glm::vec3(0, 0, 1)};

    // Specify indices
    indices = {// right
               1, 2, 6, 6, 5, 1,
               // left
               0, 4, 7, 7, 3, 0,
               // top
               4, 5, 6, 6, 7, 4,
               // bottom
               0, 3, 2, 2, 1, 0,
               // back
               0, 1, 5, 5, 4, 0,
               // front
               3, 7, 6, 6, 2, 3};

    bindgl(positions, normals, indices);
  }
};
