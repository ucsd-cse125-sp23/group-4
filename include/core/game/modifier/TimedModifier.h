#pragma once

#include "core/game/modifier/Modifier.h"

struct TimedModifierData : public ModifierData {
  unsigned long long expire = -1;
  TimedModifierData(unsigned long long duration);
};
class TimedModifier : public Modifier {
 public:
  TimedModifier();
  TimedModifier(bool serverOnly);
  void modify(Modifiable* obj, ModifierData* data) override;
  virtual void timedModify(Modifiable* obj, ModifierData* data) = 0;
};