#include "core/game/effect/ContextAwareGlobalEffect.h"

#include <set>

#include "core/util/global.h"

ContextAwareGlobalEffect::ContextAwareGlobalEffect(GlobalEffect* effect,
                                                   Targets target)
    : effect(effect), target(target) {}
void ContextAwareGlobalEffect::apply(Level* level,
                                     std::vector<PObject*> targets) {
  if (targets.empty()) return;
  bool targetTaggers = false;
  std::set<size_t> ids;
  TaggedStatusModifierData* data;
  switch (target) {
    case ContextAwareGlobalEffect::Targets::ALL:
      for (auto pair : level->players) effect->apply(level, pair.second);
      return;
    case ContextAwareGlobalEffect::Targets::SELF:
      for (auto obj : targets) effect->apply(level, obj);
      return;
    case ContextAwareGlobalEffect::Targets::OTHER:
      for (PObject* obj : targets) ids.insert(obj->id);
      for (auto pair : level->players)
        if (ids.find(pair.second->id) == ids.end())
          effect->apply(level, pair.second);
      return;
    case ContextAwareGlobalEffect::Targets::TAGGERS:
      targetTaggers = true;
      break;
    case ContextAwareGlobalEffect::Targets::NONTAGGERS:
      targetTaggers = false;
      break;
    case ContextAwareGlobalEffect::Targets::SELF_TAG_STATUS:
      if (!targets[0]->hasModifier(TAGGED_STATUS_MODIFIER)) return;
      data = static_cast<TaggedStatusModifierData*>(
          targets[0]->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get());
      targetTaggers = data->isIt;
      break;
    case ContextAwareGlobalEffect::Targets::NOT_SELF_TAG_STATUS:
      if (!targets[0]->hasModifier(TAGGED_STATUS_MODIFIER)) return;
      data = static_cast<TaggedStatusModifierData*>(
          targets[0]->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get());
      targetTaggers = !data->isIt;
      break;
  }
  for (auto pair : level->players) {
    data = static_cast<TaggedStatusModifierData*>(
        pair.second->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get());
    if (data->isIt == targetTaggers) effect->apply(level, pair.second);
  }
}
