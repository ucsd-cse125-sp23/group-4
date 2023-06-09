#pragma once

#include "core/game/effect/GlobalEffect.h"
#include "core/game/physics/PObject.h"

class PowerUp : public PObject {
 private:
  GlobalEffect* effect;

 public:
  PowerUp(vec3f pos, GlobalEffect* effect);
  void onTrigger(PObject* other) override;
};
