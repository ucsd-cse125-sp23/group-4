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

void Camera::UpdateView() { UpdateView(getWorldMtx()); }

void Camera::UpdateView(Node* parent) {
  glm::mat4 mtx = parent->transformMtx * transformMtx;
  UpdateView(mtx);  // since Node doesn't know its own parent
}

void Camera::UpdateView(glm::mat4 worldMtx) {
  //// Compute camera world matrix
  transform.updateMtx(&transformMtx);
  if (!Fixed) {
    transformMtx = glm::mat4(1);
    transformMtx[3][2] += Distance;
  }
  transformMtx = glm::eulerAngleY(glm::radians(-Azimuth)) *
                 glm::eulerAngleX(glm::radians(-Incline)) * transformMtx;

  // Compute view matrix (inverse of world matrix)
  glm::mat4 view = glm::inverse(worldMtx);

  // Compute perspective projection matrix
  glm::mat4 project =
      glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

  // Compute final view-projection matrix
  ViewMtx = view;
  ViewProjectMtx = project * view;
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
  float dist = glm::clamp(GetDistance() * (1.0f - static_cast<float>(y) * rate),
                          0.01f, 1000.0f);
  SetDistance(dist);
}

UserState Camera::update(float dt) {
  if (!Fixed) return UserState();

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

  return UserState();
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Reset() {
  FOV = 45.0f;
  Aspect = 1.33f;
  NearClip = 0.1f;
  FarClip = 500.0f;

  Distance = 10.0f;
  Azimuth = -0.0f;
  Incline = 20.0f;

  transform.position = vec3(0);

  Fixed = false;
}
