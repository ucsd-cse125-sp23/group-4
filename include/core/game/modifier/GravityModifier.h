#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"

struct GravityModifierData : ModifierData {
  float gravity;
  explicit GravityModifierData(float gravity) : gravity(gravity) {}
};
class GravityModifier : public Modifier {
 public:
  void modify(Modifiable* obj, ModifierData* data) override;
};
