#pragma once

#include <vector>

#include "core/game/event/EventManager.h"
#include "core/game/level/Environment.h"
#include "core/game/level/PObjectCollection.h"
#include "core/game/level/StatisticManager.h"
#include "core/game/physics/PObject.h"

enum class CollisionType { NONE, COLLISION, TRIGGER };
class Level {
 private:
  unsigned long long age;
  CollisionType collisionTypeLUT[10][10];
  Environment* environment;

  PObjectCollection objects;

 public:
  EventManager* eventManager;
  StatisticManager* statisticManager;

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
