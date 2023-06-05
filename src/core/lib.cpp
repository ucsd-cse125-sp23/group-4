#include "core/lib.hpp"

#include <chrono>
#include <random>

#include "core/game/level/Level.h"
#include "core/game/physics/PowerUp.h"
#include "core/math/shape/LineShape.h"

Level* initializeLevel(Environment* environment) {
  level = new Level(environment);
  level->setCollisionType(CollisionType::COLLISION, PLAYER_LAYER,
                          ENVIRONMENT_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, PLAYER_LAYER, PLAYER_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, POWER_LAYER, PLAYER_LAYER);

  return level;
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

void applyGameMode(GameMode* gamemode) {
  gamemode->registerTrackers(level);
  level->gameMode = gamemode;
}
int queryScore(uint32_t pid) { return level->gameMode->queryScore(pid); }
std::vector<std::vector<uint32_t>> queryPlacements() {
  return level->gameMode->queryPlacements();
}
void initPlayers(std::map<uint32_t, Player*> players) {
  level->gameMode->initPlayers(players);
}
