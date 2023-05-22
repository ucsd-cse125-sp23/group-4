#pragma once

#include <map>
#include <vector>

#include "core/game/event/EventManager.h"
#include "core/game/level/Environment.h"
#include "core/game/level/PObjectCollection.h"
#include "core/game/level/StatisticManager.h"
#include "core/game/physics/PObject.h"
#include "core/game/physics/Player.h"

class GameMode;
enum class CollisionType { NONE, COLLISION, TRIGGER };
class Level {
 private:
  unsigned long long age;
  CollisionType collisionTypeLUT[10][10];
  Environment* environment;

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
  void addPObject(PObject* obj);
  unsigned long long getAge();
  Environment* getEnvironment();
  void tick();
};
