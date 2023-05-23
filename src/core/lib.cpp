#include "core/lib.hpp"

#include <chrono>
#include <random>

#include "core/game/level/Level.h"
#include "core/game/physics/PowerUp.h"
#include "core/math/shape/LineShape.h"

void initializeLevel(Environment* environment) {
  level = new Level(environment);
  level->setCollisionType(CollisionType::COLLISION, PLAYER_LAYER,
                          ENVIRONMENT_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, PLAYER_LAYER, PLAYER_LAYER);
  level->setCollisionType(CollisionType::TRIGGER, POWER_LAYER, PLAYER_LAYER);
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

void spawnPowerUp(vec3f min, vec3f max,
                  const std::vector<GlobalEffect*>& effects) {
  std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  float minT = -1;
  //Sufficiently efficient ray casting
  Ray ray;
  int n = 0;
  while (n < 5) {
    ray = {vec3f(min.x + (max.x - min.x) * dist(rng), 1000.0,
                 min.z + (max.z - min.z) * dist(rng)),
           vec3f(0, -1, 0)};
    LineShape* line = new LineShape(ray.src, ray.src + ray.dir * 1e9f);
    for (auto obj : level->getEnvironment()->collides(line)) {
      float t = obj->getBounds()->intersects(ray);
      if (t >= 0 && t < minT) minT = t;
    }
    delete line;
    if (minT >= 0) break;
  }
  if (minT < 0) return;

  vec3f pos = ray.src + ray.dir * minT;
  size_t i = static_cast<size_t>(dist(rng) * effects.size());
  level->addPObject(new PowerUp(pos, effects[i]));
}
