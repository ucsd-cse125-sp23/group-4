#include "core/game/mode/GameMode.h"

#include <map>

#include "core/util/global.h"

NTaggersTimeGameMode::NTaggersTimeGameMode(int n) : GameMode(true), n(n) {
}
void NTaggersTimeGameMode::registerTrackers(Level* l) {
  GameMode::registerTrackers(l);

  level->statisticManager->registerStat("ticks_it", 0);
  level->eventManager->registerEventHandler([](TaggingEvent event) {
    if (Player* p = dynamic_cast<Player*>(event.tagger))
      p->level->statisticManager->setValue(
          p->pid, "ticks_it",
          std::get<int>(
              p->level->statisticManager->getValue(p->pid, "ticks_it")) +
              event.ticksIt);
  });
}
int NTaggersTimeGameMode::queryScore(uint32_t pid) {
  int ticksIt =
      std::get<int>(level->statisticManager->getValue(pid, "ticks_it"));
  Player* p = level->players[pid];
  TaggedStatusModifierData* data = static_cast<TaggedStatusModifierData*>(
      p->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get());
  if (data->isIt) ticksIt += level->getAge() - data->taggedTime;
  return ticksIt;
}
std::vector<Player*> NTaggersTimeGameMode::initPlayers(
    std::map<uint32_t, Player*> players) {
  std::vector<Player*> pPerm;
  for (auto pair : players) pPerm.push_back(pair.second);
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(pPerm.begin(), pPerm.end(), g);

  level->spreadPlayers(pPerm);
  for (Player* p : pPerm) {
    p->level->statisticManager->setValue(p->pid, "ticks_it", 0);
    p->level->statisticManager->setValue(p->pid, "it_count", 0);
  }
  for (int i = 0; i < n && i < players.size(); i++) {
    TaggedStatusModifierData* data = static_cast<TaggedStatusModifierData*>(
        pPerm[i]->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get());
    data->isIt = true;
    data->taggedTime = pPerm[i]->level->getAge();
    pPerm[i]->level->statisticManager->setValue(pPerm[i]->pid, "ticks_it",
                                                -TAG_COOLDOWN);
  }
}

GameMode::GameMode(bool lowerScoreLead)
    : lowerScoreLead(lowerScoreLead), level(nullptr) {}
void GameMode::registerTrackers(Level* l) {
  this->level = l;

  level->eventManager->registerEventHandler([](TriggerEvent event) {
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
  if (lowerScoreLead) {
    for (auto ite = s2p.begin(); ite != s2p.end(); ite++)
      ret.push_back((*ite).second);
  } else {
    for (auto ite = s2p.rbegin(); ite != s2p.rend(); ite++)
      ret.push_back((*ite).second);
  }
  return ret;
}
