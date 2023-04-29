////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"
#include "Input.h"

using namespace glm;

////////////////////////////////////////////////////////////////////////////////

Camera::Camera() {
  Reset();
  Fixed = false;


  _renderGizmo = false;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::UpdateView(GLFWwindow* window) {
  //// Compute camera world matrix
  transformMtx = glm::mat4(1);
  transformMtx[3][2] = Distance;
  transformMtx = glm::eulerAngleY(glm::radians(-Azimuth)) *
                 glm::eulerAngleX(glm::radians(-Incline)) * transformMtx;

  // Compute view matrix (inverse of world matrix)
  glm::mat4 view = glm::inverse(getWorldMtx());

  // Compute perspective projection matrix
  glm::mat4 project =
      glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

  // Compute final view-projection matrix
  ViewProjectMtx = project * view;
}

void Camera::update(float dt) {
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

  moveLocal = normalize(moveLocal);

  moveLocal *= 8 * dt;

  if (length(moveLocal) > 0) move_local(moveLocal);
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

  Fixed = false;
}

