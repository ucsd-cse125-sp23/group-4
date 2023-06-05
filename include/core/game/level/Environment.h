#pragma once

#include <set>
#include <utility>
#include <vector>
#include <random>

#include "core/game/physics/PObject.h"
#include "core/game/physics/Player.h"
#include "core/math/shape/AABShape.h"

class Environment {
 public:
  Environment();
  ~Environment();
  const std::vector<PObject*>& getCollisions();
  void addBox(vec3f min, vec3f max, float friction = 0.0f);
  void addConvex(std::vector<vec3f> vertices, float friction = 0.0f);
  void addConvex(std::initializer_list<vec3f> vertices, float friction = 0.0f);

  void addPlayerSpawnpoint(vec3f pos);
  void addItemSpawnpoint(vec3f pos);
  void setDeathHeight(vec3f pos);

  void placePlayers(std::mt19937& rng, std::vector<Player*> players);
  const std::vector<vec3f> getItemSpawns();
  float getDeathHeight();


  void constructBVH();
  std::vector<PObject*> collides(BoundingShape* shape);
  std::vector<PObject*> collides(PObject* self);
  std::pair<PObject*, vec4f> mtv(PObject* self);
  std::pair<PObject*, vec4f> ccd(PObject* self, vec3f dPos,
                                 std::set<PObject*> ignore = {});

  struct BVHNode {
    AABShape* bound;
    PObject* obj;
    BVHNode *left, *right;
    explicit BVHNode(AABShape* aab)
        : bound(aab), obj(nullptr), left(nullptr), right(nullptr) {}
  };

 private:
  Environment::BVHNode* root;
  std::vector<PObject*> collisions;
  std::vector<vec3f> playerSpawns;
  std::vector<vec3f> itemSpawns;
  float deathHeight;
  void addPObject(PObject* obj);
};
