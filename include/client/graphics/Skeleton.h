////////////////////////////////////////
// Skeleton.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <map>
#include <string>

#include "client/graphics/Camera.h"
#include "client/graphics/Joint.h"
#include "client/graphics/Pose.h"
#include "client/graphics/core.h"

////////////////////////////////////////////////////////////////////////////////

// The Skeleton class for holding joints of a skeleton tree structure.

class Skeleton {
 public:
  Skeleton() { root = nullptr; }
  ~Skeleton() { delete root; }

  bool Load(const char* file);
  void Update(float deltaTime);
  void Draw(const glm::mat4& viewProjMtx, GLuint shader);
  void Show();  // GUI

  glm::mat4 GetWorldMatrix(int joint);

  void SetPose(const Pose pose);

 private:
  std::vector<Joint*> joints;  // random access

  Joint* root;  // tree structure

  bool applyToRoot = false;

  void SetJointDOFs(int joint, glm::vec3 dof);
  Joint* GetJoint(int j);
  Joint* FindJointInTree(int j);
};

////////////////////////////////////////////////////////////////////////////////
