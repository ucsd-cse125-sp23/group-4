#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

struct ControlModifierData : ModifierData {
  vec3f horizontalVel;
  bool doJump;
  float jumpVel;
  explicit ControlModifierData(Level* level);
  explicit ControlModifierData(Level* level, float jumpVel);
};
class ControlModifier : public Modifier {
  void modify(Modifiable* obj, ModifierData* data) override;
};
