#pragma once

#include <set>
#include <vector>

#include "core/game/effect/GlobalEffect.h"
#include "core/game/modifier/TaggedStatusModifierData.h"
#include "core/util/global.h"

struct AttractOtherToTaggersEffect : public GlobalEffect {
 public:
  void apply(Level* level, std::vector<PObject*> targets) {
    std::set<size_t> ids;
    std::vector<PObject*> taggers;
    for (PObject* obj : targets) ids.insert(obj->id);
    for (auto pair : level->players)
      if (static_cast<TaggedStatusModifierData*>(
              pair.second->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get())
              ->isTagger)
        taggers.push_back(pair.second);
    for (auto pair : level->players)
      if (ids.find(pair.second->id) == ids.end())
        for (PObject* obj : taggers)
          pair.second->addModifierInstance(new ModifierInstance(
              ATTRACT_MODIFIER, new AttractModifierData(200, obj, 0.2)));
  }
};
struct RepellTaggersFromSelfEffect : public GlobalEffect {
 public:
  void apply(Level* level, std::vector<PObject*> targets) {
    for (auto pair : level->players)
      if (static_cast<TaggedStatusModifierData*>(
              pair.second->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get())
              ->isTagger)
        for (PObject* self : targets)
          tagger->addModifierInstance(new ModifierInstance(
              ATTRACT_MODIFIER, new AttractModifierData(200, self, -0.1)));
  }
};