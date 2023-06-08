#pragma once

#include <vector>

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"
#include "network/effect.hpp"
#include "network/item.hpp"

struct EffectInstance {
  Effect effect;
  Item item;
  size_t expire;
};

struct EffectStorageModifierData : ModifierData {
  std::vector<EffectInstance> effects;
  explicit EffectStorageModifierData(Level* level) : ModifierData(level) {}
};
class EffectStorageModifier : public Modifier {
 public:
  EffectStorageModifier();
  void modify(Modifiable* obj, ModifierData* data) override;

  static void addEffect(PObject* obj, Effect effect, size_t duration);
  static std::vector<std::pair<Item,Effect>> queryEffects(PObject* obj);
};
