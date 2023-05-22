#include "core/game/effect/GlobalEffect.h"

#include "core/util/global.h"

void GlobalEffect::apply(Level* level, PObject* target) {
  apply(level, {target});
}
