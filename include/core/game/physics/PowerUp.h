#pragma once

#include "core/game/effect/GlobalEffect.h"
#include "core/game/physics/PObject.h"
#include "network/item.hpp"
#include <random>
#include <chrono>


class PowerUp : public PObject {
 private:
  static std::mt19937 rng;

  GlobalEffect* effect;
  Item item;

 public:
  PowerUp(vec3f pos, GlobalEffect* effect);
  void onTrigger(PObject* other) override;
};
