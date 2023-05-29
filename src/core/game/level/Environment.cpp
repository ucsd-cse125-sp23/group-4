#include "core/game/level/Environment.h"

#include <stack>

#include "core/util/global.h"
#include "core/math/shape/MovementShape.h"
#include "core/math/shape/ConvexMeshShape.h"

void Environment::addPObject(PObject* obj) {
  collisions.push_back(obj);
  obj->static_ = true;
}

Environment::Environment() : root(nullptr) {}

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

inline double volume(const vec3f& v) { return v.x * v.y * v.z; }
inline int maxInd(const vec3f& v) {
  if (v.x > v.y) {
    if (v.x > v.z)
      return 0;
    else
      return 2;
  } else {
    if (v.y > v.z)
      return 1;
    else
      return 2;
  }
}
Environment::BVHNode* constructBVHHelper(
    std::vector<Environment::BVHNode*> nodes) {
  if (nodes.size() == 1) return nodes[0];
  vec3f minP = nodes[0]->bound->minP, maxP = nodes[0]->bound->maxP;
  for (Environment::BVHNode* node : nodes) {
    minP = min(minP, node->bound->minP);
    maxP = max(maxP, node->bound->maxP);
  }
  Environment::BVHNode* top = new Environment::BVHNode(new AABShape(minP, maxP));
  const int axis = maxInd(maxP - minP);
  std::sort(nodes.begin(), nodes.end(),
    [axis](Environment::BVHNode* a, Environment::BVHNode* b) {
      return a->bound->minP[axis] < b->bound->minP[axis];
    }
  );

  size_t beg = 0, end = nodes.size(), mid = nodes.size() / 2;
  while (end - beg > 4) {
    float v0, v1;
    minP = nodes[0]->bound->minP;
    maxP = nodes[0]->bound->maxP;
    for (int i = 1; i < mid; i++) {
      minP = min(minP, nodes[i]->bound->minP);
      maxP = max(maxP, nodes[i]->bound->maxP);
    }
    v0 = volume(maxP - minP);
    minP = nodes[mid]->bound->minP;
    maxP = nodes[mid]->bound->maxP;
    for (int i = mid+1; i < nodes.size(); i++) {
      minP = min(minP, nodes[i]->bound->minP);
      maxP = max(maxP, nodes[i]->bound->maxP);
    }
    v1 = volume(maxP - minP);

    if (v0 > v1) {
      end = mid;
      mid = (beg + mid) / 2;
    } else {
      beg = mid;
      mid = (mid + end) / 2;
    }
  }
  std::vector<Environment::BVHNode*> leftList =
      std::vector<Environment::BVHNode*>(nodes.begin(), nodes.begin() + mid);
  std::vector<Environment::BVHNode*> rightList =
      std::vector<Environment::BVHNode*>(nodes.begin() + mid, nodes.end());
  top->left = constructBVHHelper(leftList);
  top->right = constructBVHHelper(rightList);

  return top;
}
void Environment::constructBVH() {
  if (collisions.size() == 0) return;
  std::vector<BVHNode*> leaves;
  for (PObject* obj : collisions) {
    leaves.push_back(new BVHNode(obj->getBounds()->shape->bounds()));
    leaves.back()->obj = obj;
  }
  this->root = constructBVHHelper(leaves);
}
std::vector<PObject*> Environment::collides(BoundingShape* shape) {
  std::vector<PObject*> hits;
  if (this->root != nullptr) {
    std::stack<BVHNode*> stack;
    stack.push(this->root);
    while (!stack.empty()) {
      BVHNode* curr = stack.top();
      stack.pop();
      //curr->bound has no position offset
      if (shape->collides(curr->bound)) {
        if (curr->obj == nullptr) {
          stack.push(curr->left);
          stack.push(curr->right);
        } else {
          const CollisionBounds* bounds = curr->obj->getBounds();
          if (bounds->shape->collides(
                  shape, translate_scale(bounds->getPos(), bounds->getScale())))
            hits.push_back(curr->obj);
        }
      }
    }
  }
  return hits;
}
std::vector<PObject*> Environment::collides(PObject* self) {
  std::vector<PObject*> hits;
  if (this->root != nullptr) {
    std::stack<BVHNode*> stack;
    stack.push(this->root);
    mat4f transform = translate_scale(self->getBounds()->getPos(),
                                      self->getBounds()->getScale());
    const BoundingShape* shape = self->getBounds()->shape;
    while (!stack.empty()) {
      BVHNode* curr = stack.top();
      stack.pop();
      if (shape->collides(curr->bound, transform)) {
        if (curr->obj == nullptr) {
          stack.push(curr->left);
          stack.push(curr->right);
        } else if (self->getBounds()->collides(curr->obj->getBounds())) {
          hits.push_back(curr->obj);
        }
      }
    }
  }
  return hits;
}
std::pair<PObject*, vec4f> Environment::mtv(PObject* self) {
  vec4f minMTV = vec4f(0.0f, 0.0f, 0.0f, std::numeric_limits<float>::max());
  PObject* minObj = nullptr;
  if (this->root != nullptr) {
    std::stack<BVHNode*> stack;
    stack.push(this->root);
    mat4f transform = translate_scale(self->getBounds()->getPos(),
                                      (self->getBounds()->getScale()));
    const BoundingShape* shape = self->getBounds()->shape;
    while (!stack.empty()) {
      BVHNode* curr = stack.top();
      stack.pop();
      if (shape->collides(curr->bound, transform)) {
        if (curr->obj == nullptr) {
          stack.push(curr->left);
          stack.push(curr->right);
        } else {
          vec4f v = self->getBounds()->mtv(curr->obj->getBounds());
          if (v.w > 0)
            if (v.w < minMTV.w) {
              minMTV = v;
              minObj = curr->obj;
            }
        }
      }
    }
  }
  return std::make_pair(minObj, minMTV);
}

float Environment::ccd(PObject* self, vec3f dPos) {
  CollisionBounds* bounds = self->getBounds();
  OffsetShape* offsetShape = new OffsetShape(dynamic_cast<const ConvexShape*>(bounds->shape), bounds->getPos());
  MovementShape* moveShape = new MovementShape(offsetShape, dPos);
  std::vector<PObject*> collisions = this->collides(moveShape);
  if (collisions.empty()) return 1;

  float maxDistSqr = length_squared(dPos);
  if (maxDistSqr == 0) return 0;

  size_t ite = 0;
  //vec3f dir = normalize(dPos);
  vec3f movement = vec3f(0,0,0);
  do {
    float minT = 1;
    for (PObject* obj : collisions) {
      const CollisionBounds* bounds = obj->getBounds();
      vec3f distV = offsetShape->distance(
          bounds->shape, translate(movement),
          translate_scale(bounds->getPos(), bounds->getScale()));
      //std::cout << distV << ":" << dot(distV, distV) / dot(dPos, distV)
      //          << std::endl;
      if (length_squared(distV) == 0) {
        minT = 0;
        break;
      }
      minT = std::min(minT, dot(distV, distV) / dot(dPos, distV));
    }
    /*movement += dir * std::sqrt(minDistSqr);*/

    /*float dist = dot(normalize(minDistV), dir);
    movement += dir * dist;*/

    vec3f diff = minT * dPos;
    movement += diff;
    dPos -= diff;

    //std::cout << "movement:" << movement << std::endl;
    //std::cout << "diff:" << diff << std::endl;

    if (length_squared(diff) < TOLERANCE * TOLERANCE) break;
  } while (maxDistSqr - length_squared(movement) > TOLERANCE * TOLERANCE &&
           ++ite < MAX_ITERATIONS);
  //std::cout << ite << std::endl;
  return std::min(1.0f, sqrt(dot(movement, movement) / maxDistSqr));
}
