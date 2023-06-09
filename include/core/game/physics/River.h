#pragma once

#include "core/game/physics/PObject.h"

class River : public PObject {
 public:
  River(std::vector<vec3f> vertices);
  ~River();
  void onTrigger(PObject* other) override;
};
