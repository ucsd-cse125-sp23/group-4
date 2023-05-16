#pragma once

#include "client/graphics/Mesh.h"
#include "client/graphics/core.h"

////////////////////////////////////////////////////////////////////////////////

class Cube : public Mesh {
 public:
  explicit Cube(glm::vec3 cubeMin = glm::vec3(-1, -1, -1),
                glm::vec3 cubeMax = glm::vec3(1, 1, 1));
  ~Cube() { cleargl(); }
};

////////////////////////////////////////////////////////////////////////////////
