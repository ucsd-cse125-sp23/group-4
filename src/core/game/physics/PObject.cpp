#include "core/game/physics/PObject.h"

uint32_t PObject::maxId = 0;

PObject::PObject(BoundingShape* shape, unsigned int layer, float friction,
                 bool s)
    : bounds(new CollisionBounds(shape, layer, friction)),
      static_(s),
      pos(vec3f(0.0f, 0.0f, 0.0f)),
      oPos(vec3f(0.0f, 0.0f, 0.0f)),
      vel(vec3f(0.0f, 0.0f, 0.0f)),
      onGround(false),
      freeze(false),
      level(nullptr) {
  this->id = PObject::maxId++;
}
PObject::~PObject() { delete bounds; }
void PObject::tick() {
  Modifiable::tick();

  lastSurfaceNormal = vec3f(0, 0, 0);
  lastSurfaceFriction = 0;

  oPos = pos;
  pos.y += vel.y;
  if (!freeze) {
    pos.x += vel.x;
    pos.z += vel.z;
  }
  if (vel.y < -0.01) onGround = false;
  freeze = false;
  this->bounds->setPos(pos);
}
vec3f PObject::getPos() { return pos; }
void PObject::setPos(vec3f pos) {
  this->pos = pos;
  this->bounds->setPos(pos);
}
void PObject::addPos(vec3f dPos) { this->setPos(pos + dPos); }
bool PObject::markedRemove() { return toRemove; }
void PObject::markRemove() { toRemove = true; }

bool PObject::isStatic() { return this->static_; }
CollisionBounds* PObject::getBounds() { return this->bounds; }
