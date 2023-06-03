#include "core/game/effect/StaticGlobalEffect.h"

StaticGlobalEffect::StaticGlobalEffect(
    std::function<void(Level*, std::vector<PObject*>)> f)
    : applyTo(f) {}
void StaticGlobalEffect::apply(Level* level, std::vector<PObject*> targets) {
  applyTo(level, targets);
}
