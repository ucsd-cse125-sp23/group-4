#pragma once

#include <cstdint>
#include <vector>

#include "core/game/modifier/Modifier.h"

class ModifierInstance;
enum Operation { ADD, ADDMULTIPLY, MULTIPLY };
struct NumberModifierData : public ModifierData {
  Operation op;
  float val;
  NumberModifierData(Level* level, Operation op, float val,
                     std::uint64_t duration = 0)
      : ModifierData(level, duration), op(op), val(val) {}
};
class NumberModifier : public Modifier {
 public:
  NumberModifier();
  void modify(Modifiable* obj, ModifierData* data) override;

  static float evaluate(float base,
                        const std::vector<ModifierInstance*>& numberModifiers);
};
