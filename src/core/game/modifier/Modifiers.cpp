#include <cstdint>

#include "core/game/modifier/AttractModifier.h"
#include "core/game/modifier/ControlModifier.h"
#include "core/game/modifier/EffectStorageModifier.h"
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
    vec3f targetHeading = cData->horizontalVel;
    targetHeading.y = 0;
    vec3f norm = pObj->lastSurfaceNormal;
    if (norm != vec3f(0, 0, 0) && norm != vec3f(0, 1, 0)) {
      vec3f uphillDir = normalize(tangent(vec3f(0, 1, 0), norm));
      vec3f uphillHeading = normalize(tangent(uphillDir, vec3f(0, 1, 0)));
      float fractionUphill = dot(normalize(targetHeading), uphillHeading);
      float cosine = std::abs(norm.y);
      targetHeading -=
          uphillHeading * std::max(0.0f, fractionUphill) * (1 - cosine);
    }
    vec3f targetVel = targetHeading * MOVE_VELOCITY;
    vec3f dv = (targetVel - pObj->vel) * 0.6f;

    if (length_squared(cData->horizontalVel) < length_squared(pObj->vel))
      dv *= 0.6f;
    float fFactor =
        std::clamp(std::sqrt(pObj->modifyValue(1, FRICTION_MODIFIER)) *
                       (pObj->onGround >= COYOTE_TIME
                            ? pObj->lastSurfaceFriction *
                                  pObj->modifyValue(1, GRAVITY_MODIFIER) * 2
                            : 0.2f),
                   0.0f, 1.0f);
    dv *= fFactor;
    pObj->vel.x += dv.x;
    pObj->vel.z += dv.z;
    if (pObj->onGround && cData->doJump && !pObj->freeze) {
      vec3f dj = norm;
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
      dj *=
          cData->jumpVel * (pObj->onGround >= COYOTE_TIME ? sqrt(fFactor) : 1);
      pObj->vel.x += dj.x / 10;
      pObj->vel.y = dj.y;
      pObj->vel.z += dj.z / 10;
      pObj->onGround = 0;
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
bool TaggedStatusModifier::isIt(core::Player* player) {
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

EffectStorageModifier::EffectStorageModifier() {}
void EffectStorageModifier::modify(Modifiable* obj, ModifierData* data) {
  auto* cData = static_cast<EffectStorageModifierData*>(data);
  for (int i = 0; i < cData->effects.size(); i++) {
    if (data->level->getAge() > cData->expire) {
      cData->effects[i--] = cData->effects.back();
      cData->effects.pop_back();
    }
  }
}

void EffectStorageModifier::addEffect(PObject* obj, Effect effect,
                                      size_t duration) {
  for (auto m : obj->getModifiers(EFFECT_STORAGE_MODIFIER)) {
    static_cast<EffectStorageModifierData*>(m->get())->effects.push_back(
        {effect, obj->level->getAge() + duration});
  }
}
std::vector<Effect> EffectStorageModifier::queryEffects(PObject* obj) {
  std::vector<Effect> ret;
  for (auto m : obj->getModifiers(EFFECT_STORAGE_MODIFIER)) {
    for (auto e : static_cast<EffectStorageModifierData*>(m->get())->effects)
      ret.push_back(e.effect);
    break;
  }
  return ret;
}
