/**************************************************
 * Camera.h
 * Implementation of GameThing
 * handles rotation/movement of camera
 * and computing view projection matrices
 *****************************************************/

#pragma once

#include <functional>

#include "client/graphics/GameThing.h"
#include "client/graphics/core.h"
#include "core/game/level/Environment.h"
#include "core/math/Ray.h"
#include "glm/gtx/euler_angles.hpp"

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
////////////////////////////////////////////////////////////////////////////////

class Camera : public GameThing {
 private:
  // interpolation
  float lerpSpeed = 10.0f;
  glm::vec3 position_prev;
  glm::vec3 position_target;

 public:
  bool Fixed;

  Environment* env;
  // std::function<float(Ray)> raycastFunction;

  Camera();

  void update(float dt);

  void SetPositionTarget(glm::vec3 v) {
    position_target = v + glm::vec3(0, 4, 0);  // center above player
  }

  void UpdateView();
  void UpdateView(Node* parent);
  void UpdateView(glm::mat4 rootMtx);

  void Reset();

  // Access functions
  void SetAspect(float a) { Aspect = a; }
  void SetDistance(float d) { Distance = d; }
  void SetAzimuth(float a) { Azimuth = a; }
  void SetIncline(float i) { Incline = i; }

  void CamZoom(float y, float max = 1.0f);
  void CamDrag(float a, float i);

  float GetDistance(bool raycast = false, glm::mat4* rootMtxPtr = nullptr);
  float GetAzimuth() { return Azimuth; }
  float GetIncline() { return Incline; }

  glm::mat4 getCameraRotationMtx() {
    return glm::eulerAngleY(glm::radians(-Azimuth));
  }
  const glm::mat4& GetViewMtx() { return ViewMtx; }
  const glm::mat4& GetViewProjectMtx(bool origin = false) {
    return origin ? ViewProjectOriginMtx : ViewProjectMtx;
  }

  float FOV;  // Field of View Angle (degrees)

 private:
  // Perspective controls
  // float FOV;       // Field of View Angle (degrees)
  float Aspect;    // Aspect Ratio
  float NearClip;  // Near clipping plane distance
  float FarClip;   // Far clipping plane distance

  // Polar controls
  float Distance;  // Distance of the camera eye position to the origin (meters)
  float maxDist = 150.0f;
  float Azimuth;  // Rotation of the camera eye position around the Y axis
                  // (degrees)
  float
      Incline;  // Angle of the camera eye position over the XZ plane (degrees)

  // Computed data
  glm::mat4 ViewMtx;
  glm::mat4 ViewProjectMtx;
  glm::mat4 ViewProjectOriginMtx;  // for skybox
};

////////////////////////////////////////////////////////////////////////////////
