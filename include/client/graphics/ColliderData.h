/**************************************************
 * ColliderData.h
 * For core. Raw list of vertices.
 *****************************************************/

#pragma once

#include <string>
#include <vector>

#include "core/math/vector.h"

class ColliderData {
 public:
  std::string name;
  std::vector<vec3f> vertices;  // passed in as world space

  explicit ColliderData(std::vector<vec3f> v) { vertices = v; }
};
