#include "core/game/level/Environment.h"

#include "core/util/global.h"

void Environment::addPObject(PObject* obj) {
  collisions.push_back(obj);
  obj->static_ = true;
}

Environment::~Environment() {
  for (PObject* obj : collisions) delete obj;
}

const std::vector<PObject*>& Environment::getCollisions() { return collisions; }

void Environment::addBox(vec3f min, vec3f max, float friction) {
  BoundingShape* shape = new AABShape(min, max);
  PObject* obj = new PObject(shape, ENVIRONMENT_LAYER, friction, true);
  this->addPObject(obj);
}

void Environment::addConvex(std::vector<vec3f> vertices, float friction) {
  BoundingShape* shape = new ConvexMeshShape(vertices);
  PObject* obj = new PObject(shape, ENVIRONMENT_LAYER, friction, true);
  this->addPObject(obj);
}
void Environment::addConvex(std::initializer_list<vec3f> vertices,
                            float friction) {
  BoundingShape* shape = new ConvexMeshShape(vertices);
  PObject* obj = new PObject(shape, ENVIRONMENT_LAYER, friction, true);
  this->addPObject(obj);
}