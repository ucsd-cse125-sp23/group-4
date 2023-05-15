#include "Cube.h"

Cube::Cube(glm::vec3 cubeMin, glm::vec3 cubeMax) {
  creategl();

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;

  // Specify vertex positions
  positions = {// Front
               glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
               glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
               glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
               glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),

               // Back
               glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
               glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
               glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),
               glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),

               // Top
               glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
               glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
               glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
               glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

               // Bottom
               glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
               glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
               glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
               glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),

               // Left
               glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
               glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
               glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
               glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

               // Right
               glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
               glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
               glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
               glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z)};

  // Specify normals
  normals = {// Front
             glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),
             glm::vec3(0, 0, 1),

             // Back
             glm::vec3(0, 0, -1), glm::vec3(0, 0, -1), glm::vec3(0, 0, -1),
             glm::vec3(0, 0, -1),

             // Top
             glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0),
             glm::vec3(0, 1, 0),

             // Bottom
             glm::vec3(0, -1, 0), glm::vec3(0, -1, 0), glm::vec3(0, -1, 0),
             glm::vec3(0, -1, 0),

             // Left
             glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0),
             glm::vec3(-1, 0, 0),

             // Right
             glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0),
             glm::vec3(1, 0, 0)};

  // Specify indices
  indices = {
      0,  1,  2,  0,  2,  3,   // Front
      4,  5,  6,  4,  6,  7,   // Back
      8,  9,  10, 8,  10, 11,  // Top
      12, 13, 14, 12, 14, 15,  // Bottom
      16, 17, 18, 16, 18, 19,  // Left
      20, 21, 22, 20, 22, 23,  // Right
  };

  bindgl(positions, normals, indices);
}
