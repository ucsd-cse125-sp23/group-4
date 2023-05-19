#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"

struct TaggedStatusModifierData : ModifierData {
  int taggedTime;
  bool isIt;
  explicit TaggedStatusModifierData(bool isIt) : taggedTime(0), isIt(isIt) {}
  TaggedStatusModifierData() : TaggedStatusModifierData(false) {}
};
class TaggedStatusModifier : public Modifier {
 public:
  TaggedStatusModifier();
  void modify(Modifiable* obj, ModifierData* data) override;
};
