#pragma once

#include <cstdint>

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

struct SpeedBoostModifierData : ModifierData {
  float factorIncrease;
  SpeedBoostModifierData(Level* level, std::uint64_t duration,
                         float factorIncrease)
      : ModifierData(level, duration), factorIncrease(factorIncrease) {}
};
class SpeedBoostModifier : public Modifier {
 public:
  SpeedBoostModifier();
  void modify(Modifiable* obj, ModifierData* data) override;
};
