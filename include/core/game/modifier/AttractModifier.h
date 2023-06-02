#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

struct AttractModifierData : ModifierData {
  PObject* sink;
  float factor;
  AttractModifierData(unsigned long long duration, PObject* sink, float factor)
      : ModifierData(duration), sink(sink), factor(factor) {}
};
class AttractModifier : public Modifier {
 public:
  AttractModifier();
  void modify(Modifiable* obj, ModifierData* data) override;
};
