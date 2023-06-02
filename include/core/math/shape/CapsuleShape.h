#pragma once

#include "core/math/shape/ConvexShape.h"

static int sign(float f) {
  if (f == 0)
    return 0;
  else if (f > 0)
    return 1;
  return -1;
}
class CapsuleShape : public ConvexShape {
 private:
  float halfheight, radius;

 protected:
  vec4f furthestPoint(vec3f dir) const override { /*
    float d = dot(vec3f(0, 1, 0), dir);
    vec3f rad = dir - d * vec3f(0, 1, 0);
    return vec4f(
        normalize(rad) * radius + vec3f(0, 1, 0) * (sign(d) * halfheight),
        1.0f);*/
    if (dir.y > 0)
      return vec4f(
          vec3f(0.0f, halfheight - radius, 0.0f) + normalize(dir) * radius,
          1.0f);
    return vec4f(
        vec3f(0.0f, -halfheight + radius, 0.0f) + normalize(dir) * radius,
        1.0f);
  };

 public:
  CapsuleShape(float height, float radius)
      : halfheight(height / 2), radius(radius) {}

  AABShape* bounds() const {
    return new AABShape(vec3f(-radius, -halfheight, -radius),
                        vec3f(radius, halfheight, radius));
  }
};
