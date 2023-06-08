////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

#include "Input.h"

using glm::mat4x4;
using glm::vec3;

////////////////////////////////////////////////////////////////////////////////

Camera::Camera() {
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
    transformMtx[3][2] += GetDistance(true);
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

float Camera::GetDistance(bool raycast) {
  float d = Distance;
  if (raycast) {
    // Raycast from player to camera viewpoint! TODO
  }
  return d;
}

void Camera::CamDrag(float a, float i) {
  if (Fixed) return;  // TODO(matthew) fix

  const float rate = 0.5f;  // mouse sensitivity
  SetAzimuth(GetAzimuth() + a * rate);
  SetIncline(glm::clamp(GetIncline() - i * rate, -90.0f, 90.0f));
}

void Camera::CamZoom(float y) {
  if (Fixed) return;

  const float rate = 0.05f;
  float dist = glm::clamp(Distance * (1.0f - static_cast<float>(y) * rate),
                          6.0f, 150.0f);
  SetDistance(dist);
}

void Camera::update(float dt) {
  // interpolate camera
  position_prev = glm::lerp(position_prev, position_target, lerpSpeed * dt);

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
  FarClip = 900.0f;

  Distance = 10.0f;
  Azimuth = -0.0f;
  Incline = 20.0f;

  transform.position = vec3(0);

  Fixed = false;
}
