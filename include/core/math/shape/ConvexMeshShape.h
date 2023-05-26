#pragma once

#include <algorithm>
#include <cstring>
#include <vector>

#include "core/math/shape/ConvexShape.h"

class ConvexMeshShape : public ConvexShape {
 private:
  size_t size;
  vec3f* vertices;

 protected:
  vec4f furthestPoint(vec3f dir) const override {
    vec3f ndir = normalize(dir);
    float maxDot = dot(ndir, vertices[0]);
    int ind = 0;
    for (int i = 1; i < size; i++) {
      float d = dot(ndir, vertices[i]);
      if (d > maxDot) {
        maxDot = d;
        ind = i;
      }
    }
    return vec4f(vertices[ind], 1.0f);
  };

 public:
  ConvexMeshShape(size_t size, vec3f* v) : size(size) {
    this->vertices = reinterpret_cast<vec3f*>(malloc(sizeof(vec3f) * size));
    memcpy(this->vertices, v, sizeof(vec3f) * size);
  }
  explicit ConvexMeshShape(std::vector<vec3f> v) : size(v.size()) {
    this->vertices = reinterpret_cast<vec3f*>(malloc(sizeof(vec3f) * v.size()));
    std::copy(v.begin(), v.end(), this->vertices);
  }
  ConvexMeshShape(std::initializer_list<vec3f> v) : size(v.size()) {
    this->vertices = reinterpret_cast<vec3f*>(malloc(sizeof(vec3f) * v.size()));
    std::copy(v.begin(), v.end(), this->vertices);
  }
  ~ConvexMeshShape() { free(vertices); }
};
