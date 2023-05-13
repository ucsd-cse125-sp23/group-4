/**************************************************
 * GameThing.h
 * implementation of Node that can take logic updates
 * includes some advanced support for transformations
 * usage:
 *   - thing->transform.position = vec3()
 *   - thing->transform.updateMtx(&thing->transformMtx)
 *****************************************************/

#pragma once

#define _USE_MATH_DEFINES
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>
#include <glm/gtx/transform.hpp>
#include <utility>
#include <vector>

#include "client/graphics/Node.h"
#include "client/graphics/SceneState.h"

struct Transform {
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f);

  void updateMtx(glm::mat4* mtx) {
    // update mat4 in memory:
    glm::vec3 rotConv = glm::radians(rotation);
    *mtx = translate(position) *
           glm::eulerAngleXYZ(rotConv.x, rotConv.y, rotConv.z) *
           glm::scale(scale);
  }

  glm::mat4 getRotationMtx() {
    glm::vec3 rotConv = glm::radians(rotation);
    return glm::eulerAngleXYZ(rotConv.x, rotConv.y, rotConv.z);
  }
};

// a GameThing (tm)
class GameThing : public Node {
 public:
  int netId;  // used to connect with network/core data

  Transform transform;

  virtual void update(float dt) {
    // --- example ---
    transform.rotation += glm::vec3(0, 30 * dt, 0);  // spin on y axis
    transform.updateMtx(&transformMtx);  // needed to update node matrix
  }

  void updateFromState(SceneGameThingState state) {
    // update self
    setPosition(state.position);
    setHeading(state.heading);
  }

  // transform helpers

  void setPosition(glm::vec3 pos) {
    this->transform.position = pos;
    transform.updateMtx(&transformMtx);
  }

  void setHeading(glm::vec3 direction) {
    direction = normalize(direction);
    // aka azimuth:
    float heading = std::atan2(direction.x, direction.z) + (M_PI);

    setHeading(heading);
  }
  void setHeading(float heading) {
    // purely visual, for now (rotation not applied to transform itself)
    if (!model) return;

    model->modelMtx = glm::eulerAngleY(heading);
  }

  void move(glm::vec3 movement) {
    // movement is in world space
    this->transform.position += movement;
    transform.updateMtx(&transformMtx);
  }

  void move_local(glm::vec3 movement) {
    // movement is in world space
    this->transform.position +=
        glm::vec3(transform.getRotationMtx() * glm::vec4(movement, 1));
    transform.updateMtx(&transformMtx);
  }

  glm::vec3 getForward() {
    return glm::vec3(transform.getRotationMtx() * glm::vec4(0, 0, 1, 1));
  }
  glm::vec3 getRight() {
    return glm::vec3(transform.getRotationMtx() * glm::vec4(1, 0, 0, 1));
  }
};
