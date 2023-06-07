#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"
#include "network/effect.hpp"

#include <vector>

struct EffectInstance {
  Effect effect;
  size_t expire;
};

struct EffectStorageModifierData : ModifierData {
  std::vector<EffectInstance> effects;
  explicit EffectStorageModifierData(Level* level)
      : ModifierData(level) {}
};
class EffectStorageModifier : public Modifier {
 public:
  EffectStorageModifier();
  void modify(Modifiable* obj, ModifierData* data) override;

  static void addEffect(PObject* obj, Effect effect, size_t duration);
  static std::vector<Effect> queryEffects(PObject* obj);
};

