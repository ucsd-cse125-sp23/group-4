#include "core/game/physics/PObject.h"

#include "core/game/level/Level.h"
#include <set>

uint32_t PObject::maxId = 0;

void response(PObject* self, PObject* other, vec4f mtv) {
  vec3f norm = normalize(vec3f(mtv));
  /*If object is falling and mtv is atleast a little up, the we determine the
   * object to be onGround*/
  if (self->vel.y < 0 && norm.y > 0.1 * (std::abs(norm.x) + std::abs(norm.z)))
    self->onGround = true;
  if (other->isStatic()) {
    self->addPos(vec3f(mtv) * (mtv.w + 0.0001f));
    self->vel = tangent(self->vel, norm);
    self->vel -= self->vel * mtv.w * other->getBounds()->friction;

    self->lastSurfaceNormal = norm;
    self->lastSurfaceFriction = other->getBounds()->friction;
  } else {
    if (other->vel.y < 0 &&
        norm.y / (std::abs(norm.x) + std::abs(norm.z)) > -0.05)
      other->onGround = true;

    self->addPos(vec3f(mtv) * (mtv.w * 0.5f + 0.00005f));
    self->vel = tangent(self->vel, norm);
    self->vel -= self->vel * mtv.w * other->getBounds()->friction * 0.5f;
    other->addPos(-vec3f(mtv) * (mtv.w * 0.5f + 0.00005f));
    other->vel = tangent(other->vel, /* - */ norm);
    other->vel -= other->vel * mtv.w * self->getBounds()->friction * 0.5f;

    self->lastSurfaceNormal = norm;
    self->lastSurfaceFriction = other->getBounds()->friction * 0.5f;
    other->lastSurfaceNormal = -norm;
    other->lastSurfaceFriction = self->getBounds()->friction * 0.5f;
  }

  other->onCollision(self);
  self->onCollision(other);
}

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

  //std::cout << "::" << pos << " " << vel << " " << onGround << std::endl;

  int ite = 0;
  float totalY = 0;
  vec3f rDPos = dPos;
  std::set<PObject*> collided;
  //vec3f latestNorm = vec3f(0, 0, 0);
  while (length_squared(rDPos) > 0.01 && ite++ < 15) {
    std::pair<PObject*, vec4f> hit =
        this->level->getEnvironment()->ccd(this, rDPos, collided);
    float t = hit.second.w;
    vec3f norm = vec3f(hit.second);
    //if (norm == vec3f(0, 0, 0)) norm = latestNorm;

    vec3f mDPos = rDPos * hit.second.w;

    mDPos -= normalize(mDPos) * 0.001f;
    rDPos -= mDPos;

    //std::cout << ":" << norm << " " << rDPos << " " << mDPos << std::endl;

    pos.y += mDPos.y;
    if (!freeze) {
      pos.x += mDPos.x;
      pos.z += mDPos.z;
    }
    totalY += mDPos.y;
    if (norm != vec3f(0, 0, 0)) {
      if (this->vel.y < 0 &&
          norm.y > 0.1 * (std::abs(norm.x) + std::abs(norm.z)))
        this->onGround = true;

      vec3f v = tangent(rDPos, norm);
      float w = length(rDPos - v);
      rDPos = v;
      this->vel = tangent(this->vel, norm);
      this->vel -= this->vel * w * this->getBounds()->friction;
      rDPos -= rDPos * w * hit.first->getBounds()->friction;

      this->lastSurfaceNormal = norm;
      this->lastSurfaceFriction = hit.first->getBounds()->friction;

      collided.insert(hit.first);
    }
    this->bounds->setPos(pos);
  }
  //std::cout << std::endl;

  if (totalY < -0.01 &&
      lastSurfaceNormal.y <
          0.1 * (std::abs(lastSurfaceNormal.x) + std::abs(lastSurfaceNormal.z)))
    onGround = false;

  Environment* environment = this->level->getEnvironment();
  std::pair<PObject*, vec4f> pair = environment->mtv(this);
  while (pair.first != nullptr) {
    response(this, pair.first, pair.second);
    pair = environment->mtv(this);
  }
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
