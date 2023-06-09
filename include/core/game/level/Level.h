#pragma once

#include <map>
#include <utility>
#include <vector>

#include "core/game/event/EventManager.h"
#include "core/game/level/Environment.h"
#include "core/game/level/PObjectCollection.h"
#include "core/game/level/StatisticManager.h"
#include "core/game/mode/GameMode.h"
#include "core/game/physics/PObject.h"
#include "core/game/physics/Player.h"

class GlobalEffect;
enum class CollisionType { NONE, COLLISION, TRIGGER };
class Level {
 private:
  std::mt19937 rng;
  std::uint64_t age;
  CollisionType collisionTypeLUT[10][10];
  Environment* environment;

  size_t powerupMin = 400, powerupMax = 800;
  std::vector<std::pair<int, GlobalEffect*>> powerups;
  int totalWeight = 0;

  size_t nextPowerupTime;
  std::vector<std::pair<uint32_t, vec3f>> powerupSpawns;
  int freeSpaces;

 public:
  std::map<uint32_t, Player*> players;
  PObjectCollection objects;

  EventManager* eventManager;
  StatisticManager* statisticManager;
  GameMode* gameMode;

  /**
   * @param environment defines the environement of the level. Will be deleted
   * on level deconstruction
   */
  explicit Level(Environment* environment);
  ~Level();
  void setCollisionType(CollisionType type, int layer0, int layer1);
  CollisionType collisionType(int layer0, int layer1);
  void addPObject(PObject* obj);
  std::uint64_t getAge();
  Environment* getEnvironment();
  void tick();

  void spreadPlayers(std::vector<Player*> ps);
  void restartGame();

  void definePowerupSpawn(GlobalEffect* power, int weight = 1);
  void definePowerupDelayMin(size_t ticks);
  void definePowerupDelayMax(size_t ticks);
};
