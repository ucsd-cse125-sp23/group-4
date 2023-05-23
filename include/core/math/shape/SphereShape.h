#pragma once

#include "core/math/shape/ConvexShape.h"

class SphereShape : public ConvexShape {
 private:
  float radius;

 protected:
  vec4f furthestPoint(vec3f dir) const override {
    return vec4f(normalize(dir) * radius, 1.0f);
  };

 public:
  SphereShape(float radius) : radius(radius) {}

  AABShape* bounds() const {
    return new AABShape(vec3f(-radius, -radius, -radius),
                        vec3f(radius, radius, radius));
  }
};