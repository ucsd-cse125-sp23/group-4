////////////////////////////////////////
// Camera.h
////////////////////////////////////////

#pragma once

#include "core.h"
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
////////////////////////////////////////////////////////////////////////////////

// The Camera class provides a simple means to controlling the 3D camera. It
// could be extended to support more interactive controls. Ultimately. the
// camera sets the GL projection and viewing matrices.

class Camera {
 public:
  Camera();

  void Update(GLFWwindow* window);
  void Reset();
  void ToggleFixedCamera();
  void Move(GLFWwindow* window, float delta = 0.25);
  void KeyInput(GLFWwindow* window, float delta = 0.5);

  // Access functions
  void SetAspect(float a) { Aspect = a; }
  void SetDistance(float d) { Distance = d; }
  void SetAzimuth(float a) { Azimuth = a; }
  void SetIncline(float i) { Incline = i; }
  void UpdateWorld();

  float GetDistance() { return Distance; }
  float GetAzimuth() { return Azimuth; }
  float GetIncline() { return Incline; }
  bool GetFixed() { return Fixed; }
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
  float XTranslation;  // Left/Right Translation of Camera
  float ZTranslation;  // Forward/Back Translation of Camera
  glm::vec3 Translations;
  bool Fixed;
  glm::mat4 World;

  // Computed data
  glm::mat4 ViewProjectMtx;
};

////////////////////////////////////////////////////////////////////////////////
