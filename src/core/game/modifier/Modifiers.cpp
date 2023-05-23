#include "core/game/modifier/AttractModifier.h"
#include "core/game/modifier/ControlModifier.h"
#include "core/game/modifier/FreezeModifier.h"
#include "core/game/modifier/GravityModifier.h"
#include "core/game/modifier/SpeedBoostModifier.h"
#include "core/game/modifier/TaggedStatusModifier.h"
#include "core/game/modifier/TimedModifier.h"
#include "core/util/global.h"

Modifier::Modifier() : Modifier(true) {}
Modifier::Modifier(bool serverOnly) : serverOnly(serverOnly) {}

TimedModifierData::TimedModifierData(unsigned long long duration) {
  expire = duration == 0 ? 0 : level->getAge() + duration;
}
TimedModifier::TimedModifier() : Modifier() {}
TimedModifier::TimedModifier(bool serverOnly) : Modifier(serverOnly) {}
void TimedModifier::modify(Modifiable* obj, ModifierData* data) {
  TimedModifierData* cData = static_cast<TimedModifierData*>(data);
  if (level->getAge() >= cData->expire && cData->expire > 0)
    cData->markedRemove = true;
  timedModify(obj, data);
}

SpeedBoostModifier::SpeedBoostModifier() : TimedModifier(false) {}
void SpeedBoostModifier::timedModify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    SpeedBoostModifierData* cData = static_cast<SpeedBoostModifierData*>(data);
    pObj->addPos(cData->factorIncrease * pObj->vel * vec3f(1, 0, 1));
  }
}

void GravityModifier::timedModify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj))
    pObj->vel +=
        vec3f(0.0f, -static_cast<GravityModifierData*>(data)->gravity, 0.0f);
}

ControlModifierData::ControlModifierData()
    : ControlModifierData(JUMP_VELOCITY) {}
ControlModifierData::ControlModifierData(float jumpVel)
    : jumpVel(jumpVel), horizontalVel(vec3f(0, 0, 0)), doJump(false) {}
void ControlModifier::modify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    ControlModifierData* cData = static_cast<ControlModifierData*>(data);
    vec3f dv = (cData->horizontalVel - pObj->vel) * 0.6f;
    if (length_squared(cData->horizontalVel) < length_squared(pObj->vel))
      dv *= 0.6f;
    if (!pObj->onGround) dv *= 0.4f;
    pObj->vel.x += dv.x;
    pObj->vel.z += dv.z;
    if (pObj->onGround && cData->doJump) {
      vec3f dj = pObj->lastSurfaceNormal;
      float c = std::max(0.0f, dj.y);
      float d = (1 - c) / 0.5f;
      dj *= std::min(1.0f, d*d);
      dj.y = 1.0f / (1.0f + std::exp(-15.0f * (c - 0.36f)));
      dj = normalize(dj);
      dj *= cData->jumpVel;
      pObj->vel.x += dj.x;
      pObj->vel.y = dj.y;
      pObj->vel.z += dj.z;
      pObj->onGround = false;
    }
  }
}

TaggedStatusModifier::TaggedStatusModifier() : Modifier(false) {}
void TaggedStatusModifier::modify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    TaggedStatusModifierData* cData =
        static_cast<TaggedStatusModifierData*>(data);
    if (pObj->level->getAge() - cData->taggedTime < TAG_COOLDOWN)
      pObj->freeze = true;
    else if (cData->isIt) {
      int lvl = (pObj->level->getAge() - cData->taggedTime)/200;
      if (lvl > 5) lvl = 5;
      pObj->addPos(lvl * 0.04f * pObj->vel * vec3f(1, 0, 1));
    }
  }
}

AttractModifier::AttractModifier() : TimedModifier(false) {}
void AttractModifier::timedModify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    AttractModifierData* cData = static_cast<AttractModifierData*>(data);
    pObj->addPos(cData->factor *
                 normalize(cData->sink->getPos() - pObj->getPos()));
  }
}

FreezeModifier::FreezeModifier() : TimedModifier(false) {}
void FreezeModifier::timedModify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    pObj->freeze = true;
  }
}