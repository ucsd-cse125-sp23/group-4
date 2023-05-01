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
  transform.updateMtx(&transformMtx);
  if (!Fixed) {
    transformMtx = glm::mat4(1);
    transformMtx[3][2] += Distance;
  }
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

void Camera::CamDrag(float a, float i) {
  if (Fixed) return;    // TODO fix

  const float rate = 0.5f;  // mouse sensitivity TODO
  SetAzimuth(GetAzimuth() + a * rate);
  SetIncline(glm::clamp(GetIncline() - i * rate, -90.0f, 90.0f));
}

void Camera::CamZoom(float y) {
  if(Fixed) return;

  const float rate = 0.05f;
  float dist = glm::clamp(GetDistance() * (1.0f - (float)y * rate),
                          0.01f, 1000.0f);
  SetDistance(dist);
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

  moveLocal = -moveLocal;   // invert directions
  moveLocal = normalize(moveLocal);

  moveLocal *= 12 * dt;

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

  transform.position = vec3(0);

  Fixed = false;
}

