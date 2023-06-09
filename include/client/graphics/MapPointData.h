/**************************************************
 * MapPointData.h
 * For core/server. Parsed from a mapdata.obj file.
 * Contains a set of vertices.
 * Useful for colliders or similar static map objects.
 *****************************************************/

#pragma once

#include <string>
#include <vector>

#include "core/math/vector.h"

class MapPointData {
 public:
  std::string name;
  std::vector<vec3f> vertices;  // passed in as world space
  vec3f point;                  // used for spawn points

  explicit MapPointData(std::vector<vec3f> v) {
    if (v.size() <= 0) {
      return;
    }

    vec3f lowest = v[0];
    for (auto p : v) {
      if (p.y < lowest.y) lowest = p;
    }
    point = lowest;

    vertices = v;
  }
};
