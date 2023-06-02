#pragma once

#include <set>
#include <vector>

#include "core/game/physics/PObject.h"
#include "core/math/shape/AABShape.h"

class Environment {
 public:
  Environment();
  ~Environment();
  const std::vector<PObject*>& getCollisions();
  void addBox(vec3f min, vec3f max, float friction = 0.0f);
  void addConvex(std::vector<vec3f> vertices, float friction = 0.0f);
  void addConvex(std::initializer_list<vec3f> vertices, float friction = 0.0f);

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
    BVHNode(AABShape* aab)
        : bound(aab), obj(nullptr), left(nullptr), right(nullptr) {}
  };

 private:
  Environment::BVHNode* root;
  std::vector<PObject*> collisions;
  void addPObject(PObject* obj);
};
