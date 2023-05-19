#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"

struct GravityModifierData : TimedModifierData {
  float gravity;
  GravityModifierData(float gravity, unsigned long long duration = 0) : TimedModifierData(duration), gravity(gravity) {}
};
class GravityModifier : public TimedModifier {
 public:
    void timedModify(Modifiable* obj, ModifierData* data) override;
};
