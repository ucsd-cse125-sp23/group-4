#include "core/game/effect/GlobalEffect.h"

#include "core/util/global.h"

void GlobalEffect::apply(Level* level, PObject* target) {
  std::vector<PObject*> v;
  v.push_back(target);
  apply(level, v);
}
