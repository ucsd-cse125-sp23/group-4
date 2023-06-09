#pragma once

#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

namespace core {

class Player : public PObject {
 private:
  static uint32_t maxId;

 public:
  vec3f look;
  uint32_t pid;
  Player();
  ~Player();
  void tick();
  void onTrigger(PObject* other) override;
};

}  // namespace core
