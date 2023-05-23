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

  AABShape* bounds() const {
    AABShape* base = original->bounds();
    AABShape* mod =
        new AABShape(vec3f(offset) + base->minP, vec3f(offset) + base->maxP);
    delete base;
    return mod;
  }
};
