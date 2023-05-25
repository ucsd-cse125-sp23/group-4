#include "core/lib.hpp"

#include "core/game/level/Level.h"

void initializeLevel(Environment* environment) {
  level = new Level(environment);
  level->setCollisionType(CollisionType::COLLISION, PLAYER_LAYER,
                          ENVIRONMENT_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, PLAYER_LAYER, PLAYER_LAYER);

  level->statisticManager->registerStat("ticks_it", 0);
  level->statisticManager->registerStat("it_count", 0);

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
  level->eventManager->registerTaggingEventHandler([](TaggingEvent event) {
    if (Player* p = dynamic_cast<Player*>(event.tagee))
      level->statisticManager->setValue(
          p->pid, "it_count",
          std::get<int>(level->statisticManager->getValue(p->pid, "it_count")) +
              1);
    if (Player* p = dynamic_cast<Player*>(event.tagger))
      level->statisticManager->setValue(
          p->pid, "ticks_it",
          std::get<int>(level->statisticManager->getValue(p->pid, "ticks_it")) +
              event.ticksIt);
  });
}

std::pair<Player*, ControlModifierData*> initializePlayer() {
  Player* player = new Player();
  ControlModifierData* controlData = new ControlModifierData();
  player->addModifierInstance(
      new ModifierInstance(CONTROL_MODIFIER, controlData));
  level->addPObject(player);
  return std::make_pair(player, controlData);
}

void terminateLevel() { delete level; }
