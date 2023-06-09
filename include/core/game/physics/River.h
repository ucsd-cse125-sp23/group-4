#pragma once

#include "core/game/physics/PObject.h"

class River : public PObject {
 public:
  River();
  ~River();
  void onTrigger(PObject* other) override;
};
