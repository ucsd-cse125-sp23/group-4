/**************************************************
 * Collider.h
 * Special implementation of Node
 * Just a holder for collision data (vertices) currently.
 * TODO: integrate collision detection
 *****************************************************/
#define GLM_FORCE_RADIANS

#pragma once

#include <stdio.h>

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "client/graphics/Mesh.h"
#include "client/graphics/Node.h"
#include "core/math/vector.h"

class Collider : public Node {
 public:
  std::vector<glm::vec3> vertices;  // passed in as world space

  Collider(std::vector<glm::vec3> v) { vertices = v; }

  Collider(std::vector<glm::vec3> v, Mesh* displayMesh) : Collider(v) {
    vertices = v;
  }

  void draw_debug(const glm::mat4& viewProjMtx, const glm::mat4& modelMtx,
                  bool gizmos, Model* gizmo_mdl, Model* gizmoCube_mdl) {
    // render in the vertices!
    if (gizmos && _renderGizmo) {
      for (glm::vec3 v : vertices) {
        gizmoCube_mdl->draw(
            viewProjMtx,
            modelMtx * glm::translate(v) * glm::scale(glm::vec3(0.15f)), true);
      }
    }
  }

  std::vector<vec3f> core_vertices() {
    std::vector<vec3f> varr;

    for (glm::vec3 v : vertices) {
      varr.push_back(vec3f(v.x, v.y, v.z));
    }

    return varr;
  }
};
