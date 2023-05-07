////////////////////////////////////////
// Vertex.h (UNUSED)
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include "core.h"
using namespace ::glm;

////////////////////////////////////////////////////////////////////////////////

// The Vertex struct, holds info a single point, to be constructed into a
// Triangle.

struct Vertex {
  vec3 position = vec3(0.0f);
  vec3 normal = vec3(0.0f);

  std::vector<std::pair<int, float>> skinweights;

  Vertex() {}
  Vertex(vec3 p) { position = p; }

  void SetPosition(vec3 p) { position = p; }

  void SetNormal(vec3 n) { normal = n; }

  void AddWeight(int j, float w) {
    skinweights.push_back(std::make_pair(j, w));
  }

  const vec3 GetPosition(void) { return position; }
};

////////////////////////////////////////////////////////////////////////////////
