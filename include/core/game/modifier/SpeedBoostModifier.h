#pragma once

#include <cstdint>

#include "core/game/modifier/TimedModifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"
#include "core/game/physics/PObject.h"
#include "core/game/modifier/TimedModifier.h"

struct SpeedBoostModifierData : TimedModifierData {
  float factorIncrease;
  SpeedBoostModifierData(uint64_t expire, float factorIncrease)
      : TimedModifierData(expire), factorIncrease(factorIncrease) {}
};
class SpeedBoostModifier : public TimedModifier {
 public:
  SpeedBoostModifier();
  void timedModify(Modifiable* obj, ModifierData* data) override;
};
