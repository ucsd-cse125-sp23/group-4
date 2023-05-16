#include "core/lib.hpp"

#include <random>
#include "core/game/level/Level.h"
#include "core/game/physics/PowerUp.h"

void initializeLevel(Environment* environment) {
  level = new Level(environment);
  level->setCollisionType(CollisionType::COLLISION, PLAYER_LAYER, ENVIRONMENT_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, PLAYER_LAYER, PLAYER_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, POWER_LAYER, PLAYER_LAYER);

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

void spawnPowerUp(vec3f min, vec3f max, const std::vector<GlobalEffect*>& effects) {
    std::mt19937 rng; std::uniform_real_distribution<double> dist(0.0, 1.0);
    float minT = -1;
    Ray ray;
    int n = 0;
    while(n < 5)
    {
        ray = { vec3f(min.x + (max.x - min.x) * dist(rng), 1000.0, min.z + (max.z - min.z) * dist(rng)) , vec3f(0,-1,0) };
        for (auto obj : level->getEnvironment()->getCollisions())
        {
            float t = obj->getBounds()->intersects(ray);
            if (t >= 0 && t < minT)
                minT = t;
        }
        if (minT >= 0)
            break;
    }
    if (minT < 0)
        return;

    vec3f pos = ray.src + ray.dir * minT;
    size_t i = static_cast<size_t>(dist(rng) * effects.size());
    level->addPObject(new PowerUp(pos, effects[i]));
}
