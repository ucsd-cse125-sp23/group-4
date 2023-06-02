#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

struct SpeedBoostModifierData : ModifierData {
  float factorIncrease;
  SpeedBoostModifierData(unsigned long long duration, float factorIncrease)
      : ModifierData(duration), factorIncrease(factorIncrease) {}
};
class SpeedBoostModifier : public Modifier {
 public:
  SpeedBoostModifier();
  void modify(Modifiable* obj, ModifierData* data) override;
};
