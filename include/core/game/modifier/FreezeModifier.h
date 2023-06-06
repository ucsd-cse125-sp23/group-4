#pragma once

#include <cstdint>

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

struct FreezeModifierData : ModifierData {
  explicit FreezeModifierData(Level* level, std::uint64_t duration)
      : ModifierData(level, duration) {}
};
class FreezeModifier : public Modifier {
 public:
  FreezeModifier();
  void modify(Modifiable* obj, ModifierData* data) override;
};
