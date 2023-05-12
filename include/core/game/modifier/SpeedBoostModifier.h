#pragma once

#include "core/game/modifier/TimedModifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

struct SpeedBoostModifierData : TimedModifierData {
  float factorIncrease;
  SpeedBoostModifierData(unsigned long long expire, float factorIncrease)
      : TimedModifierData(expire), factorIncrease(factorIncrease) {}
};
class SpeedBoostModifier : public TimedModifier {
 public:
  SpeedBoostModifier();
  void timedModify(Modifiable* obj, ModifierData* data) override;
};
