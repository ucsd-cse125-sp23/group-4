#include "core/game/modifier/ControlModifier.h"
#include "core/game/modifier/GravityModifier.h"
#include "core/game/modifier/SpeedBoostModifier.h"
#include "core/game/modifier/TaggedStatusModifier.h"
#include "core/game/modifier/TimedModifier.h"
#include "core/util/global.h"

Modifier::Modifier() : Modifier(true) {}
Modifier::Modifier(bool serverOnly) : serverOnly(serverOnly) {}

TimedModifier::TimedModifier() : Modifier() {}
TimedModifier::TimedModifier(bool serverOnly) : Modifier(serverOnly) {}
void TimedModifier::modify(Modifiable* obj, ModifierData* data) {
  TimedModifierData* cData = static_cast<TimedModifierData*>(data);
  if (level->getAge() >= cData->expire) cData->markedRemove = true;
}

SpeedBoostModifier::SpeedBoostModifier() : TimedModifier(false) {}
void SpeedBoostModifier::timedModify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    SpeedBoostModifierData* cData = static_cast<SpeedBoostModifierData*>(data);
    pObj->addPos(cData->factorIncrease * pObj->vel * vec3f(1, 0, 1));
  }
}

void GravityModifier::modify(Modifiable* obj, ModifierData* data) {
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
    vec3f dv =
        clampBySign(cData->horizontalVel, cData->horizontalVel - pObj->vel) *
        0.6f;
    pObj->vel += vec3f(dv.x, 0.0f, dv.z);
    if (pObj->onGround && cData->doJump) {
      pObj->vel.y = cData->jumpVel;
      pObj->onGround = false;
    }
  }
}

TaggedStatusModifier::TaggedStatusModifier() : Modifier(false) {}
void TaggedStatusModifier::modify(Modifiable* obj, ModifierData* data) {}
