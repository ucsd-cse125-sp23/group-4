#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"

struct TaggedStatusModifierData : ModifierData {
  int taggedTime;
  bool isIt;
  explicit TaggedStatusModifierData(Level* level, bool isIt)
      : ModifierData(level), taggedTime(0), isIt(isIt) {}
  explicit TaggedStatusModifierData(Level* level)
      : TaggedStatusModifierData(level, false) {}
};
class TaggedStatusModifier : public Modifier {
 public:
  TaggedStatusModifier();
  void modify(Modifiable* obj, ModifierData* data) override;
};
