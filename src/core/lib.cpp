#include "core/lib.hpp"

#include <chrono>
#include <random>

#include "core/game/level/Level.h"
#include "core/game/physics/PowerUp.h"
#include "core/math/shape/LineShape.h"

Level* initializeLevel(Environment* environment) {
  Level* level = new Level(environment);
  level->setCollisionType(CollisionType::COLLISION, PLAYER_LAYER,
                          ENVIRONMENT_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, PLAYER_LAYER, PLAYER_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, POWER_LAYER, PLAYER_LAYER);

  return level;
}

std::pair<Player*, ControlModifierData*> initializePlayer(Level* level) {
  Player* player = new Player();
  ControlModifierData* controlData = new ControlModifierData(level);
  player->addModifierInstance(
      new ModifierInstance(CONTROL_MODIFIER, controlData));
  level->addPObject(player);
  return std::make_pair(player, controlData);
}

void applyGameMode(Level* level, GameMode* gamemode) {
  gamemode->registerTrackers(level);
  level->gameMode = gamemode;
}
int queryScore(Level* level, uint32_t pid) { return level->gameMode->queryScore(pid); }
std::vector<std::vector<uint32_t>> queryPlacements(Level* level) {
  return level->gameMode->queryPlacements();
}
void initPlayers(Level* level, std::map<uint32_t, Player*> players) {
  level->gameMode->initPlayers(players);
}
