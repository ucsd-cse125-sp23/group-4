/**************************************************
 * Camera.h
 * Implementation of GameThing
 * handles rotation/movement of camera
 * and computing view projection matrices
 *****************************************************/

#pragma once

#include "client/graphics/GameThing.h"
#include "client/graphics/core.h"
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
 public:
  bool Fixed;

  Camera();

  UserState update(float dt);

  void UpdateView(GLFWwindow* window);
  void Reset();

  // Access functions
  void SetAspect(float a) { Aspect = a; }
  void SetDistance(float d) { Distance = d; }
  void SetAzimuth(float a) { Azimuth = a; }
  void SetIncline(float i) { Incline = i; }

  void CamZoom(float y);
  void CamDrag(float a, float i);

  float GetDistance() { return Distance; }
  float GetAzimuth() { return Azimuth; }
  float GetIncline() { return Incline; }

  glm::mat4 getCameraRotationMtx() {
    return glm::eulerAngleY(glm::radians(-Azimuth));
  }
  const glm::mat4& GetViewMtx() { return ViewMtx; }
  const glm::mat4& GetViewProjectMtx() { return ViewProjectMtx; }

 private:
  // Perspective controls
  float FOV;       // Field of View Angle (degrees)
  float Aspect;    // Aspect Ratio
  float NearClip;  // Near clipping plane distance
  float FarClip;   // Far clipping plane distance

  // Polar controls
  float Distance;  // Distance of the camera eye position to the origin (meters)
  float Azimuth;   // Rotation of the camera eye position around the Y axis
                   // (degrees)
  float
      Incline;  // Angle of the camera eye position over the XZ plane (degrees)

  // Computed data
  glm::mat4 ViewMtx;
  glm::mat4 ViewProjectMtx;
};

////////////////////////////////////////////////////////////////////////////////
