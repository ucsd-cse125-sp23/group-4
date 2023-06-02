#include "core/game/level/Level.h"

#include "core/util/global.h"
#include "core/game/level/Environment.h"


void Level::tick() {
  std::vector<size_t> allIds = this->objects.getAllIds();
  for (size_t id : allIds) {
    PObject* obj = this->objects[id];
    obj->tick();
    if (obj->markedRemove()) {
      this->objects.removeById(id);
    }
  }

  allIds = this->objects.getAllIds();
  std::vector<PObject*> collisions;
  for (size_t id : allIds) {
    PObject* self = objects[id];
    const CollisionBounds* selfBounds = self->getBounds();
    for (size_t id_other : allIds) {
      if (id == id_other) continue;
      PObject* obj = objects[id_other];
      switch (collisionTypeLUT[selfBounds->layer][obj->getBounds()->layer]) {
        case CollisionType::NONE:
        case CollisionType::TRIGGER:
          break;
        case CollisionType::COLLISION:
          collisions.push_back(obj);
          break;
      }
    }
    while (!collisions.empty()) {
      size_t ind = -1;
      vec4f minMTV = vec4f(0.0f, 0.0f, 0.0f, std::numeric_limits<float>::max());

      for (int k = 0; k < collisions.size(); k++) {
        vec4f mtv = selfBounds->mtv(collisions[k]->getBounds());
        if (mtv.w > 0) {
          if (mtv.w < minMTV.w) {
            ind = k;
            minMTV = mtv;
          }
        } else {
          collisions[k--] = collisions.back();
          collisions.pop_back();
        }
      }
      if (collisions.empty() || ind == -1) break;
      PObject::response(self, collisions[ind], minMTV);
      collisions[ind] = collisions.back();
      collisions.pop_back();
    }
  }

  for (size_t id : allIds) {
    PObject* self = objects[id];
    const CollisionBounds* selfBounds = self->getBounds();

    for (size_t id_other : allIds) {
      if (id == id_other) continue;
      PObject* obj = objects[id_other];
      switch (collisionTypeLUT[selfBounds->layer][obj->getBounds()->layer]) {
        case CollisionType::NONE:
        case CollisionType::COLLISION:
          break;
        case CollisionType::TRIGGER:
          if (selfBounds->collides(obj->getBounds())) {
            self->onTrigger(obj);
            obj->onTrigger(self);
            this->eventManager->fireTriggerEvent(self, obj);
          }
          break;
      }
    }
  }

  this->age++;
}

Level::Level(Environment* environment)
    : environment(environment), age(TAG_COOLDOWN), objects(PObjectCollection()), gameMode(nullptr) {
  environment->constructBVH();
  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++) collisionTypeLUT[i][j] = CollisionType::NONE;
  eventManager = new EventManager(this);
  statisticManager = new StatisticManager();
}
Level::~Level() {
  delete environment;
  delete eventManager;
  delete statisticManager;
}
void Level::setCollisionType(CollisionType type, int layer0, int layer1) {
  collisionTypeLUT[layer0][layer1] = type;
  collisionTypeLUT[layer1][layer0] = type;
}
CollisionType Level::collisionType(int layer0, int layer1) {
  return collisionTypeLUT[layer0][layer1];
}
void Level::addPObject(PObject* obj) {
  objects.addPObject(obj);
  obj->level = this;
  if (auto player = dynamic_cast<Player*>(obj)) players[player->pid] = player;
}
unsigned long long Level::getAge() { return age; }
Environment* Level::getEnvironment() { return environment; }