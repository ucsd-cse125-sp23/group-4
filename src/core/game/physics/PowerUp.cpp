#include "core/game/physics/PowerUp.h"

#include "core/util/global.h"

std::mt19937 PowerUp::rng =
    std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());

PowerUp::PowerUp(vec3f pos, GlobalEffect* effect)
    : PObject(POWERUP_BASE_SHAPE, POWER_LAYER), effect(effect) {
  this->setPos(pos);
}

void PowerUp::onTrigger(PObject* other) {
  effect->apply(level, other);
  this->markRemove();
  this->level->eventManager->firePickupEvent(other);
}
