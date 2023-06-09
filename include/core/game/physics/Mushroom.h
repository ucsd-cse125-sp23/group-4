#pragma once

#include "core/game/physics/PObject.h"

class Mushroom : public PObject {
 public:
  Mushroom();
  ~Mushroom();
  void onTrigger(PObject* other) override;
};
