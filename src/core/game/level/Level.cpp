#include "core/game/level/Level.h"

#include <chrono>
#include <cstdint>

#include "core/game/level/Environment.h"
#include "core/game/physics/PowerUp.h"
#include "core/util/global.h"

using core::Player;

void Level::tick() {
  // Tick & remove POjbects
  std::vector<size_t> allIds = this->objects.getAllIds();
  for (size_t id : allIds) {
    PObject* obj = this->objects[id];
    obj->tick();
    if (obj->markedRemove()) {
      this->objects.removeById(id);

      // Frees up powerup spawnpoint
      for (auto pair : powerupSpawns) {
        if (pair.first == id) pair.first = 0;
        freeSpaces++;
      }
    }
  }

  // Handle remaining collisions
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

  // Respawn out of bounds Players
  for (size_t id : allIds) {
    PObject* obj = this->objects[id];
    if (Player* player = dynamic_cast<Player*>(obj)) {
      if (player->getPos().y < this->environment->getDeathHeight()) {
        this->environment->placePlayers(rng, {player});
        player->vel = vec3f(0, 0, 0);
      }
    }
  }

  // Spawn Powerups
  if (powerups.size() > 0) {
    if (age > nextPowerupTime) {
      std::uniform_int_distribution dist(powerupMin, powerupMax);
      nextPowerupTime = age + dist(rng);

      std::uniform_int_distribution weight(0, totalWeight - 1);
      int budget = weight(rng);
      size_t ind = -1;
      do {
        budget -= powerups[ind].first;
      } while (budget >= 0);
      GlobalEffect* effect = powerups[ind].second;

      weight = std::uniform_int_distribution(1, freeSpaces);
      budget = weight(rng);

      ind = -1;
      while (budget > 0) {
        ind++;
        if (!powerupSpawns[ind].first) budget--;
      }

      freeSpaces--;

      PowerUp* power = new PowerUp(powerupSpawns[ind].second, effect);
      powerupSpawns[ind].first = power->id;

      this->addPObject(power);
    }
  }

  this->age++;
}

Level::Level(Environment* environment)
    : environment(environment),
      age(TAG_COOLDOWN),
      objects(PObjectCollection()),
      gameMode(nullptr) {
  environment->constructBVH();
  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++) collisionTypeLUT[i][j] = CollisionType::NONE;
  eventManager = new EventManager(this);
  statisticManager = new StatisticManager();

  rng =
      std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
  for (vec3f pos : environment->getItemSpawns())
    powerupSpawns.push_back(std::make_pair(0, pos));
  freeSpaces = powerupSpawns.size();
}
Level::~Level() {
  delete environment;
  delete eventManager;
  delete statisticManager;
  if (gameMode != nullptr) delete gameMode;
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
  for (auto pair : obj->getModifiers())
    for (auto inst : pair.second) inst->get()->level = this;
}
std::uint64_t Level::getAge() { return age; }
Environment* Level::getEnvironment() { return environment; }

void Level::spreadPlayers(std::vector<Player*> ps) {
  this->environment->placePlayers(rng, ps);
}
std::vector<Player*> Level::restartGame() {
  this->age = TAG_COOLDOWN;
  if (gameMode != nullptr) return this->gameMode->initPlayers(players);
  return {};
}

void Level::definePowerupSpawn(GlobalEffect* power, int weight) {
  totalWeight += weight;
  powerups.push_back(std::make_pair(weight, power));
}
void Level::definePowerupDelayMin(size_t ticks) { powerupMin = ticks; }
void Level::definePowerupDelayMax(size_t ticks) { powerupMax = ticks; }
