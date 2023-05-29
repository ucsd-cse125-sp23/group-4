#pragma once

#include "core/math/shape/ConvexShape.h"
#include "core/math/shape/OffsetShape.h"

class MovementShape : public ConvexShape {
 private:
  const ConvexShape *shape0, *shape1;
  vec3f dPos;

 protected:
  vec4f furthestPoint(vec3f dir) const override {
    if (dot(dir, dPos) > 0) return shape1->furthestPoint(dir);
    return shape0->furthestPoint(dir);
  };

 public:
  MovementShape(const ConvexShape* shape, vec3f dPos)
      : shape0(shape), shape1(new OffsetShape(shape, dPos)), dPos(dPos) {}
  ~MovementShape() { delete shape1; }

  AABShape* bounds() const {
    AABShape *start = shape0->bounds(), *end = shape1->bounds();
    AABShape* ret = new AABShape(min(start->minP, end->maxP), max(start->maxP, end->maxP));
    delete start;
    delete end;
    return ret;
  }
};
