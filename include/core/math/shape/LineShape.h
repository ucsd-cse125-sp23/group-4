#pragma once

#include <algorithm>

#include "core/math/shape/AABShape.h"
#include "core/math/shape/ConvexShape.h"

class LineShape : public ConvexShape {
 private:
 protected:
  vec4f furthestPoint(vec3f dir) const override {
    if (dot(a - b, dir) > 0) return vec4f(a, 1.0f);
    return vec4f(b, 1.0f);
  };

 public:
  vec3f a, b;
  LineShape(vec3f a, vec3f b) : a(a), b(b) {}

  AABShape* bounds() const { return new AABShape(min(a, b), max(a, b)); }
};
