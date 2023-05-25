#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"
#include "core/game/physics/PObject.h"
#include "core/game/modifier/Modifier.h"

struct ControlModifierData : ModifierData {
  vec3f horizontalVel;
  bool doJump;
  float jumpVel;
  ControlModifierData();
  explicit ControlModifierData(float jumpVel);
};
class ControlModifier : public Modifier {
  void modify(Modifiable* obj, ModifierData* data) override;
};
