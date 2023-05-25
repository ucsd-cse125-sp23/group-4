#pragma once

#include <cstdint>

#include "core/game/modifier/Modifier.h"


struct TimedModifierData : public ModifierData {
  uint64_t expire = -1;
  explicit TimedModifierData(uint64_t expire) : expire(expire) {}
};
class TimedModifier : public Modifier {
 public:
  TimedModifier();
  explicit TimedModifier(bool serverOnly);
  void modify(Modifiable* obj, ModifierData* data) override;
  virtual void timedModify(Modifiable* obj, ModifierData* data) = 0;
};
