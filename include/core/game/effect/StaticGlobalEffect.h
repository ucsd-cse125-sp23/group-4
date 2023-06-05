#pragma once

#include <functional>
#include <vector>

#include "core/game/effect/GlobalEffect.h"

struct StaticGlobalEffect : public GlobalEffect {
 private:
  std::function<void(Level*, std::vector<PObject*>)> applyTo;

 public:
  explicit StaticGlobalEffect(
      std::function<void(Level*, std::vector<PObject*>)> f);
  void apply(Level* level, std::vector<PObject*> targets) override;
};
