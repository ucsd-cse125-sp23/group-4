#pragma once

#include <vector>

#include "core/game/effect/GlobalEffect.h"

struct ContextAwareGlobalEffect : public GlobalEffect {
 public:
  /*
   * ALL - target all players
   * TAGGERS - target players who are taggers
   * NONTAGGERS - target players who are not taggers
   * SELF - target the list of PObject*s provided
   * OTHER - target all players not in the PObject* vector parameter
   * SELF_TAG_STATUS - target all players with the same it/notit
   *					 status as the first PObject* in the
   *vector parameter NOT_SELF_TAG_STATUS - target all players with a different
   *it/notit status as the first PObject* in the vector parameter
   *
   */
  enum class Targets {
    ALL,
    TAGGERS,
    NONTAGGERS,
    SELF,
    OTHER,
    SELF_TAG_STATUS,
    NOT_SELF_TAG_STATUS
  };

  ContextAwareGlobalEffect(GlobalEffect* effect, Targets target);
  void apply(Level* level, std::vector<PObject*> targets) override;

 private:
  GlobalEffect* effect;
  Targets target;
};
