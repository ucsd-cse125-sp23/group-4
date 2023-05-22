#pragma once

#include <functional>

#include "core/game/effect/GlobalEffect.h"

struct StaticGlobalEffect : public GlobalEffect {
 private:
  std::function<void(Level*, std::vector<PObject*>)> applyTo;

 public:
  StaticGlobalEffect(std::function<void(Level*, std::vector<PObject*>)> f);
  void apply(Level* level, std::vector<PObject*> targets) override;
};