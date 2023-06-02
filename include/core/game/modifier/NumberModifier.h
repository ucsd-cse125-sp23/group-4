#pragma once

#include <vector>

#include "core/game/modifier/Modifier.h"

class ModifierInstance;
enum Operation { ADD, ADDMULTIPLY, MULTIPLY };
struct NumberModifierData : public ModifierData {
  Operation op;
  float val;
  NumberModifierData(Operation op, float val, unsigned long long duration = 0)
      : ModifierData(duration), op(op), val(val) {}
};
class NumberModifier : public Modifier {
 public:
  NumberModifier();
  virtual void modify(Modifiable* obj, ModifierData* data) override;

  static float evaluate(float base,
                        const std::vector<ModifierInstance*>& numberModifiers);
};