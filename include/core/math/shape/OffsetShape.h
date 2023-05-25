#pragma once

#include "core/math/shape/ConvexShape.h"

class OffsetShape : public ConvexShape {
 private:
  ConvexShape* original;
  vec4f offset;

 public:
  OffsetShape(ConvexShape* original, vec3f offset) : original(original) {
    this->offset = vec4f(offset, 0.0f);
  }
  vec4f furthestPoint(vec3f dir) const override {
    return original->furthestPoint(dir) + offset;
  }
};
