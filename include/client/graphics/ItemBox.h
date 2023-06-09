#pragma once

#include <utility>

#include "GameThing.h"
#include "ParticleSystem.h"

class ItemBox : public GameThing {
 public:
  float animSpeed = 1;

  ParticleSystem* fx;
  SoundEffect* sfx;

  ItemBox() {
    fx = nullptr;
    sfx = nullptr;
  }

  ~ItemBox() {
    if (fx) delete fx;
    if (sfx) delete sfx;
  }

  void update(float dt) override;
};
