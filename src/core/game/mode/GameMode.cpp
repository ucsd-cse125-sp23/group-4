#include "core/game/mode/GameMode.h"

#include <map>

#include "core/util/global.h"

GameMode::GameMode(bool lowerScoreLead)
    : lowerScoreLead(lowerScoreLead), level(nullptr) {}
void GameMode::registerTrackers(Level* l) {
  this->level = l;

  level->eventManager->registerTriggerEventHandler([](TriggerEvent event) {
    if (event.self->hasModifier(TAGGED_STATUS_MODIFIER) &&
        event.other->hasModifier(TAGGED_STATUS_MODIFIER)) {
      TaggedStatusModifierData
          *tsself = static_cast<TaggedStatusModifierData*>(
              (event.self->getModifiers(TAGGED_STATUS_MODIFIER)[0])->get()),
          *tsother = static_cast<TaggedStatusModifierData*>(
              (event.other->getModifiers(TAGGED_STATUS_MODIFIER)[0])->get());
      if (tsself->isIt ^ tsother->isIt) {
        if (event.level->getAge() - tsself->taggedTime > TAG_COOLDOWN &&
            event.level->getAge() - tsother->taggedTime > TAG_COOLDOWN) {
          tsself->isIt = !tsself->isIt;
          tsother->isIt = !tsother->isIt;
          if (tsself->isIt) {
            tsself->taggedTime = event.level->getAge();
            event.level->eventManager->fireTaggingEvent(
                event.other, event.self,
                event.level->getAge() - tsother->taggedTime);
          } else {
            tsother->taggedTime = event.level->getAge();
            event.level->eventManager->fireTaggingEvent(
                event.self, event.other,
                event.level->getAge() - tsself->taggedTime);
          }
        }
      }
    }
  });
}
std::vector<std::vector<uint32_t>> GameMode::queryPlacements() {
  std::map<int, std::vector<uint32_t>> s2p;
  for (auto pair : level->players) {
    int score = this->queryScore(pair.first);
    if (s2p.find(score) == s2p.end()) s2p[score] = std::vector<uint32_t>();
    s2p[score].push_back(pair.first);
  }
  std::vector<std::vector<uint32_t>> ret;
  if (lowerScoreLead)
    for (auto ite = s2p.begin(); ite != s2p.end(); ite++)
      ret.push_back((*ite).second);
  else
    for (auto ite = s2p.rbegin(); ite != s2p.rend(); ite++)
      ret.push_back((*ite).second);
  return ret;
}
