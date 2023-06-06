#include "core/game/physics/PObject.h"

#include <set>

#include "core/game/level/Level.h"
#include "core/game/modifier/NumberModifier.h"
#include "core/util/global.h"

uint32_t PObject::maxId = 1;

void PObject::response(PObject* self, PObject* other, vec4f mtv) {
  vec3f norm = normalize(vec3f(mtv));
  /*If object is falling and mtv is atleast a little up, the we determine the
   * object to be onGround*/
  if (self->vel.y < 0 && norm.y > 0.1 * (std::abs(norm.x) + std::abs(norm.z))) {
    if (!self->onGround) self->level->eventManager->fireLandEvent(self);
    self->onGround = true;
  }
  if (other->isStatic()) {
    self->addPos(vec3f(mtv) * (mtv.w + 0.0001f));
    self->vel = tangent(self->vel, norm);
    self->vel -=
        self->vel * mtv.w *
        self->modifyValue(other->getBounds()->friction, FRICTION_MODIFIER);

    self->lastSurfaceNormal = norm;
    self->lastSurfaceFriction = other->getBounds()->friction;
  } else {
    if (other->vel.y < 0 &&
        norm.y / (std::abs(norm.x) + std::abs(norm.z)) > -0.05) {
      if (!self->onGround) self->level->eventManager->fireLandEvent(self);
      self->onGround = true;
    }

    self->addPos(vec3f(mtv) * (mtv.w * 0.5f + 0.00005f));
    self->vel = tangent(self->vel, norm);
    self->vel -=
        self->vel * mtv.w *
        self->modifyValue(other->getBounds()->friction, FRICTION_MODIFIER) *
        0.5f;
    other->addPos(-vec3f(mtv) * (mtv.w * 0.5f + 0.00005f));
    other->vel = tangent(other->vel, /* - */ norm);
    other->vel -=
        other->vel * mtv.w *
        other->modifyValue(self->getBounds()->friction, FRICTION_MODIFIER) *
        0.5f;

    self->lastSurfaceNormal = norm;
    self->lastSurfaceFriction = other->getBounds()->friction * 0.5f;
    other->lastSurfaceNormal = -norm;
    other->lastSurfaceFriction = self->getBounds()->friction * 0.5f;
  }

  other->onCollision(self);
  self->onCollision(other);
  self->level->eventManager->fireCollisionEvent(self, other);
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
      lastSurfaceNormal(vec3f(0, 0, 0)),
      lastSurfaceFriction(0) {
  this->id = PObject::maxId++;
}
PObject::~PObject() { delete bounds; }
void PObject::tick() {
  Modifiable::tick();
  this->vel.y -= std::max(0.0f, PObject::modifyValue(1.0f, GRAVITY_MODIFIER));

  lastSurfaceNormal = vec3f(0, 0, 0);
  lastSurfaceFriction = 0;

  move(this->vel);
  freeze = false;
}
void PObject::move(vec3f dPos) {
  oPos = pos;

  int ite = 0;
  float totalY = 0;
  vec3f rDPos = dPos;
  std::set<PObject*> collided;
  while (length_squared(rDPos) > 0.01 && ite++ < 15) {
    std::pair<PObject*, vec4f> hit =
        this->level->getEnvironment()->ccd(this, rDPos, collided);
    float t = hit.second.w;
    vec3f norm = vec3f(hit.second);

    vec3f mDPos = rDPos * hit.second.w;

    mDPos -= normalize(mDPos) * 0.001f;
    rDPos -= mDPos;

    pos.y += mDPos.y;
    if (!freeze) {
      pos.x += mDPos.x;
      pos.z += mDPos.z;
    }
    totalY += mDPos.y;
    if (norm != vec3f(0, 0, 0)) {
      if (this->vel.y < 0 &&
          norm.y > 0.1 * (std::abs(norm.x) + std::abs(norm.z))) {
        if (!this->onGround) this->level->eventManager->fireLandEvent(this);
        this->onGround = true;
      }

      vec3f v = tangent(rDPos, norm);
      float w = length(rDPos - v);
      rDPos = v;
      this->vel = tangent(this->vel, norm);
      float baseMu = hit.first->getBounds()->friction;
      float modifiedMu = this->modifyValue(baseMu, FRICTION_MODIFIER);
      this->vel = this->vel * std::max(0.0f, 1.0f - w * modifiedMu);
      rDPos = rDPos * std::max(0.0f, 1.0f - w * modifiedMu);

      this->lastSurfaceNormal = norm;
      this->lastSurfaceFriction = baseMu;

      collided.insert(hit.first);
    }
    this->bounds->setPos(pos);
  }
  for (PObject* obj : collided) {
    obj->onCollision(this);
    this->onCollision(obj);
    level->eventManager->fireCollisionEvent(this, obj);
  }

  if (totalY < -0.01 &&
      lastSurfaceNormal.y <
          0.1 * (std::abs(lastSurfaceNormal.x) + std::abs(lastSurfaceNormal.z)))
    onGround = false;

  Environment* environment = this->level->getEnvironment();
  std::pair<PObject*, vec4f> pair = environment->mtv(this);
  while (pair.first != nullptr && ite++ < 20) {
    response(this, pair.first, pair.second);
    pair = environment->mtv(this);
  }

  const CollisionBounds* selfBounds = this->getBounds();
  for (PObject* obj : environment->collides(this))
    if (level->collisionType(selfBounds->layer, obj->getBounds()->layer) ==
        CollisionType::TRIGGER) {
      this->onTrigger(obj);
      obj->onTrigger(this);
      level->eventManager->fireTriggerEvent(this, obj);
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

float PObject::modifyValue(float value, NumberModifier* modifier) {
  return NumberModifier::evaluate(value, this->getModifiers(modifier));
}

bool PObject::isStatic() { return this->static_; }
CollisionBounds* PObject::getBounds() { return this->bounds; }
