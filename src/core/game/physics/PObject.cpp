#include "core/game/physics/PObject.h"

#include "core/game/level/Level.h"
#include <set>

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
      level(nullptr),
      lastSurfaceNormal(vec3f(0,0,0)),
      lastSurfaceFriction(0) {
  this->id = PObject::maxId++;
}
PObject::~PObject() { delete bounds; }
void PObject::tick() {
  Modifiable::tick();

  //if (!onGround)
  //  lastSurfaceNormal = vec3f(0, 0, 0);
  lastSurfaceFriction = 0;
  
  move(this->vel);
  freeze = false;
}
void PObject::move(vec3f dPos) {
  oPos = pos;

  std::cout << "::" << pos << " " << vel << " " << onGround << std::endl;

  int ite = 0;
  float totalY = 0;
  vec3f rDPos = dPos;
  std::set<PObject*> collided;
  vec3f latestNorm = vec3f(0, 0, 0);
  while (length_squared(rDPos) > 0.01 && ite++ < 15) {
    std::pair<PObject*, vec4f> hit =
        this->level->getEnvironment()->ccd(this, rDPos, collided);
    float t = hit.second.w;
    vec3f norm = vec3f(hit.second);
    if (norm == vec3f(0, 0, 0)) norm = latestNorm;

    vec3f mDPos = rDPos * hit.second.w;
    rDPos -= mDPos;

    pos.y += mDPos.y;
    if (!freeze) {
      pos.x += mDPos.x;
      pos.z += mDPos.z;
    }
    totalY += mDPos.y;
    std::cout << ":" << norm << " " << t << " " << rDPos << std::endl;
    if (norm != vec3f(0, 0, 0)) {
      rDPos = tangent(rDPos, norm);
      this->vel = tangent(this->vel, norm);
      // other->vel -= other->vel * mtv.w * self->getBounds()->friction * 0.5f;
      if (this->vel.y < 0 &&
          norm.y > 0.05 * (std::abs(norm.x) + std::abs(norm.z)))
        this->onGround = true;

      this->lastSurfaceNormal = norm;
      collided.clear();
      collided.insert(hit.first);
    }
    this->bounds->setPos(pos);

  }
  std::cout << std::endl;

  //if (totalY < -0.01) onGround = false;
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
