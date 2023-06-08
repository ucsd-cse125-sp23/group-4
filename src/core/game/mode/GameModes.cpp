#include "core/game/mode/GameModes.h"

#include "core/util/global.h"

OneTaggerCountGameMode::OneTaggerCountGameMode()
    : NTaggerGameMode(1), GameMode(true) {}
void OneTaggerCountGameMode::registerTrackers(Level* level) {
  GameMode::registerTrackers(level);
  CountGameMode::registerTrackers(level);
}

OneTaggerTimeGameMode::OneTaggerTimeGameMode()
    : NTaggerGameMode(1), GameMode(true) {}
void OneTaggerTimeGameMode::registerTrackers(Level* level) {
  GameMode::registerTrackers(level);
  TimeGameMode::registerTrackers(level);
}

ThreeTaggersTimeGameMode::ThreeTaggersTimeGameMode()
    : NTaggerGameMode(3), GameMode(true) {}
void ThreeTaggersTimeGameMode::registerTrackers(Level* level) {
  GameMode::registerTrackers(level);
  TimeGameMode::registerTrackers(level);
}

NTaggersTimeGameMode::NTaggersTimeGameMode(int n)
    : NTaggerGameMode(n), GameMode(true) {}
void ThreeTaggersTimeGameMode::registerTrackers(Level* level) {
  GameMode::registerTrackers(level);
  TimeGameMode::registerTrackers(level);
}

TeamedTaggersTimeGameMode::TeamedTaggersTimeGameMode()
    : TeamedTaggerGameMode(), GameMode(true) {}
void TeamedTaggersTimeGameMode::registerTrackers(Level* l) {
  this->level = l;
  TimeGameMode::registerTrackers(level);

  level->eventManager->registerEventHandler([=](TriggerEvent event) {
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
          for (int i = 0; i < teams[0].size(); i++) {
            PObject *team0Obj = level->objects[teams[0][i]],
                    *team1Obj = level->objects[teams[1][i]];
            tsself = static_cast<TaggedStatusModifierData*>(
                (team0Obj->getModifiers(TAGGED_STATUS_MODIFIER)[0])->get());
            tsother = static_cast<TaggedStatusModifierData*>(
                (team1Obj->getModifiers(TAGGED_STATUS_MODIFIER)[0])->get());
            tsself->isIt = !tsself->isIt;
            tsother->isIt = !tsother->isIt;
            if (tsself->isIt) {
              tsself->taggedTime = event.level->getAge();
              event.level->eventManager->fireTaggingEvent(
                  team1Obj, team0Obj,
                  event.level->getAge() - tsother->taggedTime);
            } else {
              tsother->taggedTime = event.level->getAge();
              event.level->eventManager->fireTaggingEvent(
                  team0Obj, team1Obj,
                  event.level->getAge() - tsself->taggedTime);
            }
          }
        }
      }
    }
  });
}
