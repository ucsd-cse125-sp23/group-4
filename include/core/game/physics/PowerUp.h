#pragma once

#include <chrono>
#include <random>

#include "core/game/effect/GlobalEffect.h"
#include "core/game/physics/PObject.h"
#include "network/item.hpp"

class PowerUp : public PObject {
 private:
  static std::mt19937 rng;

  GlobalEffect* effect;

 public:
  PowerUp(vec3f pos, GlobalEffect* effect);
  void onTrigger(PObject* other) override;

  Item item;
};
