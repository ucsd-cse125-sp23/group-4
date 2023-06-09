#pragma once

#include <utility>
#include <vector>

#include "core/game/modifier/Modifiable.h"
#include "core/game/modifier/ModifierInstance.h"
#include "core/game/physics/CollisionBounds.h"
#include "core/math/vector.h"

class NumberModifier;
class Level;
class PObject : public Modifiable {
 private:
  static uint32_t maxId;
  bool toRemove = false;
  vec3f pos;
  CollisionBounds* bounds;

 public:
  uint32_t id;
  vec3f vel, oPos;
  int onGround;
  int ticksFallen;
  bool static_, freeze;
  Level* level;

  vec3f lastSurfaceNormal;
  float lastSurfaceFriction;

  PObject(BoundingShape* shape, unsigned int layer = 0, float friction = 0.0f,
          bool static_ = false);
  ~PObject();
  vec3f getPos();
  void setPos(vec3f pos);
  void addPos(vec3f dPos);
  bool markedRemove();
  void markRemove();
  bool isStatic();
  CollisionBounds* getBounds();
  virtual void tick();
  virtual void move(vec3f dPos);
  virtual void onCollision(PObject* other) {}
  virtual void onTrigger(PObject* other) {}

  float modifyValue(float value, NumberModifier* modifier);

  static void response(PObject* self, PObject* other, vec4f mtv);
};
