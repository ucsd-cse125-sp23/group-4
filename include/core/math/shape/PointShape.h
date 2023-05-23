#pragma once

#include "core/math/shape/ConvexShape.h"

class PointShape : public ConvexShape {
 private:
  vec3f point;

 protected:
  vec4f furthestPoint(vec3f dir) const override { return vec4f(point, 1.0f); };

 public:
  PointShape(vec3f point) : point(point) {}

  AABShape* bounds() const { return new AABShape(point, point); }
};