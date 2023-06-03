#pragma once

#include <cstdint>

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

struct FreezeModifierData : ModifierData {
  explicit FreezeModifierData(std::uint64_t duration)
      : ModifierData(duration) {}
};
class FreezeModifier : public Modifier {
 public:
  FreezeModifier();
  void modify(Modifiable* obj, ModifierData* data) override;
};
