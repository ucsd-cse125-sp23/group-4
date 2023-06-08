#include <cstdint>

#include "core/game/modifier/AttractModifier.h"
#include "core/game/modifier/ControlModifier.h"
#include "core/game/modifier/FreezeModifier.h"
#include "core/game/modifier/NumberModifier.h"
#include "core/game/modifier/SpeedBoostModifier.h"
#include "core/game/modifier/TaggedStatusModifier.h"
#include "core/util/global.h"

ModifierData::ModifierData(Level* level, std::uint64_t duration)
    : level(level), markedRemove(false) {
  expire = duration == 0 ? 0 : level->getAge() + duration;
}
void ModifierData::markExpired() {
  if (this->level->getAge() >= this->expire && this->expire > 0)
    this->markedRemove = true;
}
Modifier::Modifier() {}

SpeedBoostModifier::SpeedBoostModifier() {}
void SpeedBoostModifier::modify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    SpeedBoostModifierData* cData = static_cast<SpeedBoostModifierData*>(data);
    pObj->addPos(cData->factorIncrease * pObj->vel * vec3f(1, 0, 1));
  }
}

ControlModifierData::ControlModifierData(Level* level)
    : ControlModifierData(level, JUMP_VELOCITY) {}
ControlModifierData::ControlModifierData(Level* level, float jumpVel)
    : ModifierData(level),
      jumpVel(jumpVel),
      horizontalVel(vec3f(0, 0, 0)),
      doJump(false) {}
void ControlModifier::modify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    ControlModifierData* cData = static_cast<ControlModifierData*>(data);
    vec3f dv = (cData->horizontalVel * MOVE_VELOCITY - pObj->vel) * 0.6f;
    if (length_squared(cData->horizontalVel) < length_squared(pObj->vel))
      dv *= 0.6f;
    dv *= std::clamp(
        std::sqrt(pObj->modifyValue(1, FRICTION_MODIFIER)) *
            (pObj->onGround ? pObj->lastSurfaceFriction *
                                  pObj->modifyValue(1, GRAVITY_MODIFIER) * 25
                            : 0.3f),
        0.0f, 1.0f);
    pObj->vel.x += dv.x;
    pObj->vel.z += dv.z;
    if (pObj->onGround && cData->doJump) {
      vec3f dj = pObj->lastSurfaceNormal;
      if (dj.y < 0) {
        dj.x = 0;
        dj.z = 0;
      } else if (dj.y < 0.1) {
        dj = vec3f(0, 0, 0);
      }
      float c = std::max(0.0f, dj.y);
      float d = (1 - c) / 0.5f;
      dj *= std::min(1.0f, d * d);
      dj.y = 1.0f / (1.0f + std::exp(-15.0f * (c - 0.36f)));
      dj = normalize(dj);
      dj *= cData->jumpVel;
      pObj->vel.x += dj.x;
      pObj->vel.y = dj.y;
      pObj->vel.z += dj.z;
      pObj->onGround = false;
      pObj->level->eventManager->fireJumpEvent(pObj);
    }
  }
}

TaggedStatusModifier::TaggedStatusModifier() {}
void TaggedStatusModifier::modify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    TaggedStatusModifierData* cData =
        static_cast<TaggedStatusModifierData*>(data);
    if (pObj->level->getAge() - cData->taggedTime < TAG_COOLDOWN) {
      pObj->freeze = true;
    } else if (cData->isIt) {
      int lvl = (pObj->level->getAge() - cData->taggedTime) / 200;
      if (lvl > 5) lvl = 5;
      pObj->addPos(lvl * 0.04f * pObj->vel * vec3f(1, 0, 1));
    }
  }
}
bool TaggedStatusModifier::isIt(Player* player) {
  return static_cast<TaggedStatusModifierData*>(
             player->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get())
      ->isIt;
}

AttractModifier::AttractModifier() {}
void AttractModifier::modify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    AttractModifierData* cData = static_cast<AttractModifierData*>(data);
    pObj->addPos(cData->factor *
                 normalize(cData->sink->getPos() - pObj->getPos()));
  }
}

FreezeModifier::FreezeModifier() {}
void FreezeModifier::modify(Modifiable* obj, ModifierData* data) {
  if (PObject* pObj = dynamic_cast<PObject*>(obj)) {
    pObj->freeze = true;
  }
}

NumberModifier::NumberModifier() {}
void NumberModifier::modify(Modifiable* obj, ModifierData* data) {}
float NumberModifier::evaluate(
    float base, const std::vector<ModifierInstance*>& numberModifiers) {
  float add = 0, mul = 1;
  for (auto inst : numberModifiers) {
    auto* cData = static_cast<NumberModifierData*>(inst->get());
    switch (cData->op) {
      case ADD:
        add += cData->val;
        break;
      case ADDMULTIPLY:
        mul += cData->val;
        break;
      case MULTIPLY:
        base *= cData->val;
        break;
    }
  }
  return base * mul + add;
}
