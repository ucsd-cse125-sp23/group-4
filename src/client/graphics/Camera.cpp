////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

#include <SFML/Audio.hpp>

#include "Input.h"

using glm::mat4x4;
using glm::vec3;

////////////////////////////////////////////////////////////////////////////////

Camera::Camera() {
  env = nullptr;

  Reset();
  Fixed = false;

  _renderGizmo = false;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::UpdateView() {
  glm::mat4 root = glm::translate(position_prev);
  UpdateView(root);
}

void Camera::UpdateView(Node* parent) {
  UpdateView(parent->transformMtx);  // since Node doesn't know its own parent
}

void Camera::UpdateView(glm::mat4 rootMtx) {
  //// Compute camera world matrix
  transform.updateMtx(&transformMtx);  // shouldn't be in draw but oh well
  if (!Fixed) {
    transformMtx = glm::mat4(1);
    transformMtx[3][2] += GetDistance(true, &rootMtx);
  }
  transformMtx = glm::eulerAngleY(glm::radians(-Azimuth)) *
                 glm::eulerAngleX(glm::radians(-Incline)) * transformMtx;

  // Compute view matrix (inverse of world matrix)
  glm::mat4 view = glm::inverse(rootMtx * transformMtx);

  // Compute perspective projection matrix
  glm::mat4 project =
      glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

  // Compute final view-projection matrix
  ViewMtx = view;
  ViewProjectMtx = project * view;
  ViewProjectOriginMtx = project * glm::mat4(glm::mat3(view));
}

float Camera::GetDistance(bool raycast, glm::mat4* rootMtxPtr) {
  float d = Distance;
  if (raycast && rootMtxPtr && env) {
    // Raycast from player to camera viewpoint!
    Ray plyrToCam = Ray();
    plyrToCam.src = vec3f(position_prev.x, position_prev.y, position_prev.z);

    glm::mat4 camMtx = glm::mat4(1);
    camMtx[3][2] += maxDist + 2.0f;

    camMtx = glm::eulerAngleY(glm::radians(-Azimuth)) *
             glm::eulerAngleX(glm::radians(-Incline)) * camMtx;
    camMtx = *rootMtxPtr * camMtx;
    glm::vec3 camPos = glm::vec3(camMtx[3]);

    glm::vec3 dir = camPos - position_prev;

    plyrToCam.dir = vec3f(dir.x, dir.y, dir.z);

    // float dResult = raycastFunction(plyrToCam);
    float dResult = 1000.0f;

    env->intersectsLoop(plyrToCam, &dResult);

    d = dir.length() * dResult;
    // gameEnv->intersects(plyrToCam, &d);
  }
  return std::min(Distance, d);
}

void Camera::CamDrag(float a, float i) {
  if (Fixed) return;  // TODO(matthew) fix

  const float rate = 0.5f;  // mouse sensitivity
  SetAzimuth(GetAzimuth() + a * rate);
  SetIncline(glm::clamp(GetIncline() - i * rate, -90.0f, 90.0f));
}

void Camera::CamZoom(float y, float max) {
  if (Fixed) return;

  const float rate = 0.05f;
  float dist = glm::clamp(Distance * (1.0f - static_cast<float>(y) * rate),
                          6.0f, std::max(maxDist, max));
  SetDistance(dist);
}

void Camera::update(float dt) {
  // interpolate camera
  position_prev = glm::lerp(position_prev, position_target, lerpSpeed * dt);
  sf::Listener::setPosition(position_target.x, position_target.y,
                            position_target.z);
  if (!Fixed) return;

  vec3 moveLocal = vec3(0);

  // read inputs for fixed camera debug movement
  if (Input::GetInputState(InputAction::MoveForward) != InputState::None) {
    moveLocal += vec3(0, 0, 1);
  }
  if (Input::GetInputState(InputAction::MoveBack) != InputState::None) {
    moveLocal += vec3(0, 0, -1);
  }
  if (Input::GetInputState(InputAction::MoveRight) != InputState::None) {
    moveLocal += vec3(-1, 0, 0);
  }
  if (Input::GetInputState(InputAction::MoveLeft) != InputState::None) {
    moveLocal += vec3(1, 0, 0);
  }

  moveLocal = -moveLocal;  // invert directions
  moveLocal = normalize(moveLocal);

  moveLocal *= 12 * dt;

  if (length(moveLocal) > 0) move_local(moveLocal);

  return;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Reset() {
  FOV = 45.0f;
  Aspect = 1.33f;
  NearClip = 0.1f;
  FarClip = 1500.0f;

  Distance = 10.0f;
  Azimuth = -0.0f;
  Incline = 20.0f;

  transform.position = vec3(0);

  Fixed = false;
}
