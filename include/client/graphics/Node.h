/**************************************************
 * Node.h
 * the basic building block of the scene hierarchy
 * leave "model" null to be treated as a point in space
 * only draw methods. see GameThing.h for updated things
 *****************************************************/

#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "client/graphics/Model.h"

// a GameThing (tm)
// drawable, with a model and child nodes. maybe extend this class later
class Node {
 public:
  std::string name = "node";
  Node() {}
  explicit Node(std::string name) { this->name = name; }

  glm::mat4 transformMtx;  // local space
  glm::mat4
      parentMtxCache;  // world space of parent (before multiplying the above)

  glm::mat4 getWorldMtx() { return parentMtxCache * transformMtx; }

  Model* model = nullptr;
  bool skybox = false;

  std::vector<Node*> childnodes;

  virtual void draw(DrawInfo info, const glm::mat4& parentMtx) {
    parentMtxCache = parentMtx;
    if (model) {
      model->draw(info.GetVP(skybox), info.viewMtx, parentMtx * transformMtx);
    }
  }

  virtual void draw_debug(DrawInfo info, const glm::mat4& parentMtx,
                          bool gizmos, Model* gizmo_mdl, Model* gizmoCube_mdl) {
    if (gizmos && _renderGizmo) {
      gizmo_mdl->draw(info.viewProjMtx, info.viewMtx, parentMtx * transformMtx);
    }
  }

  // flags to be used elsewhere
  bool _enabled = true;
  bool _renderGizmo = true;
};
