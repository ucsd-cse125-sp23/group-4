#pragma once

#include <cstdint>

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

struct LaunchModifierData : ModifierData {
  bool boosted;
  float boost;
  vec3f pos;
  LaunchModifierData(Level* level, float boost, vec3f pos)
      : ModifierData(level, 3), boosted(false), boost(boost), pos(pos) {}
};
class LaunchModifier : public Modifier {
 public:
  LaunchModifier();
  void modify(Modifiable* obj, ModifierData* data) override;
};
