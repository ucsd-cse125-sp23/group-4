#include "core/lib.hpp"

#include <chrono>
#include <random>

#include "core/game/level/Level.h"
#include "core/game/physics/PowerUp.h"
#include "core/math/shape/LineShape.h"

using core::Player;

Level* initializeLevel(Environment* environment) {
  Level* level = new Level(environment);
  level->setCollisionType(CollisionType::COLLISION, PLAYER_LAYER,
                          ENVIRONMENT_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, PLAYER_LAYER, PLAYER_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, POWER_LAYER, PLAYER_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, PLAYER_LAYER,
                          ENVIRONMENTAL_EFFECT_LAYER);

  return level;
}

std::pair<Player*, ControlModifierData*> initializePlayer(Level* level) {
  Player* player = new Player();
  ControlModifierData* controlData = new ControlModifierData(level);
  player->addModifierInstance(
      new ModifierInstance(CONTROL_MODIFIER, controlData, 0));
  level->addPObject(player);
  return std::make_pair(player, controlData);
}

void applyGameMode(Level* level, GameMode* gamemode) {
  gamemode->registerTrackers(level);
  level->gameMode = gamemode;
}
int queryScore(Level* level, uint32_t pid) {
  return level->gameMode->queryScore(pid);
}
std::vector<std::vector<uint32_t>> queryPlacements(Level* level) {
  return level->gameMode->queryPlacements();
}
void initPlayers(Level* level, std::map<uint32_t, Player*> players) {
  level->gameMode->initPlayers(players);
}

std::vector<PowerUp*> getPowerUps(Level* level) {
  std::vector<PowerUp*> ret;
  for (size_t id : level->objects.getAllIds())
    if (PowerUp* p = dynamic_cast<PowerUp*>(level->objects[id]))
      ret.push_back(p);
  return ret;
}

bool isMoving(Player* player) {
  return length_squared(static_cast<ControlModifierData*>(
                            player->getModifiers(CONTROL_MODIFIER)[0]->get())
                            ->horizontalVel) > 0.1;
}
