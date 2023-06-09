#pragma once

#include "core/game/physics/PObject.h"

class Mushroom : public PObject {
 public:
  Mushroom(std::vector<vec3f> vertices);
  ~Mushroom();
  void onTrigger(PObject* other) override;
};
