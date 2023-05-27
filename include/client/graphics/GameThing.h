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

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>
#include <glm/gtx/transform.hpp>
#include <network/message.hpp>
#include <network/tcp_server.hpp>
#include <utility>
#include <vector>

#include "client/graphics/Node.h"

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
  int netId = -1;  // used to connect with network/core data (-1 means unset)
  bool isUser = false;  // controls whether we want to read input and send it

  Transform transform;
  float azimuth = 0;  // for visuals only (aka heading)

  virtual void update(float dt) {
    // --- example (spin spin) ---
    transform.rotation += glm::vec3(0, 30 * dt, 0);  // spin on y axis
    transform.updateMtx(&transformMtx);  // needed to update node matrix
  }

  virtual message::UserStateUpdate pollInput() {
    return message::UserStateUpdate();
  }

  void updateFromState(message::GameStateUpdateItem state) {
    // update self from server input
    glm::vec3 pos = glm::vec3(state.posx, state.posy, state.posz);
    setPositionTarget(pos);
    setHeading(state.heading);
  }

  // transform helpers
  const float t_rate =
      std::chrono::duration_cast<std::chrono::duration<double>>(
          Server::TICK_RATE)
          .count() +
      0.015f;  // in seconds (50ms tickrate + lag)
  glm::vec3 position_p;
  glm::vec3 position_t;
  float lerp_p = 0.0f;
  void updateInterpolate(float dt) {
    lerp_p += dt;
    transform.position = glm::lerp(position_p, position_t,
                                   std::clamp(lerp_p / t_rate, 0.0f, 1.0f));
    transform.updateMtx(&transformMtx);
  }
  void setPositionTarget(glm::vec3 pos) {
    // interpolated!
    position_p = position_t;
    position_t = pos;
    lerp_p = 0.0f;
  }

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
    azimuth = heading;

    // purely visual, for now (rotation not applied to transform itself)
    if (!model) return;

    model->modelMtx = glm::eulerAngleY(azimuth);
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
