#pragma once

#include "core/math/shape/ConvexShape.h"

class ExpandedShape : public ConvexShape {
 private:
  const ConvexShape* original;
  float f;

 public:
  ExpandedShape(const ConvexShape* original, float f)
      : original(original), f(f) { }
  vec4f furthestPoint(vec3f dir) const override {
    return original->furthestPoint(dir) + vec4f(normalize(dir) * f, 0.0f);
  }

  AABShape* bounds() const {
    AABShape* base = original->bounds();
    AABShape* mod = new AABShape(base->minP - f,
                                base->maxP + f);
    delete base;
    return mod;
  }
};
