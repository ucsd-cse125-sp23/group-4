#pragma once

#include "core/math/shape/BoundingShape.h"

class CollisionBounds {
 private:
  vec3f pos, scale;

 public:
  const BoundingShape* shape;
  const unsigned int layer;
  const float friction;
  CollisionBounds(BoundingShape* shape, unsigned int layer, float friction);

  void setPos(vec3f pos);
  void setScale(vec3f scale);
  vec3f getPos() const;
  vec3f getScale() const;
  bool collides(const CollisionBounds* other) const;
  vec4f mtv(const CollisionBounds* other) const;
  bool contains(vec3f point) const;
  float intersects(Ray ray) const;
};
