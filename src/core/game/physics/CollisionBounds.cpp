#include "core/game/physics/CollisionBounds.h"

CollisionBounds::CollisionBounds(BoundingShape* shape, unsigned int layer,
                                 float friction)
    : pos(vec3f(0, 0, 0)),
      scale(vec3f(1, 1, 1)),
      shape(shape),
      layer(layer),
      friction(friction) {}
void CollisionBounds::setPos(vec3f pos) { this->pos = pos; }
void CollisionBounds::setScale(vec3f scale) { this->scale = scale; }
vec3f CollisionBounds::getPos() const { return this->pos; }
vec3f CollisionBounds::getScale() const { return this->scale; }

bool CollisionBounds::collides(const CollisionBounds* other) const {
  return this->shape->collides(other->shape,
                               translate_scale(this->pos, this->scale),
                               translate_scale(other->pos, other->scale));
}
vec4f CollisionBounds::mtv(const CollisionBounds* other) const {
  return this->shape->mtv(other->shape, translate_scale(this->pos, this->scale),
                          translate_scale(other->pos, other->scale));
}
bool CollisionBounds::contains(vec3f point) const {
  return this->shape->contains(point, translate_scale(this->pos, this->scale));
}
float CollisionBounds::intersects(Ray ray) const {
  return this->shape->intersects(ray, translate_scale(this->pos, this->scale));
}
