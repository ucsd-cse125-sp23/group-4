////////////////////////////////////////
// Joint.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Cube.h"
#include "DOF.h"
#include "imported/Tokenizer.h"

////////////////////////////////////////////////////////////////////////////////

// The Joint class, acts like a tree

class Joint {
 public:
  Joint() { box = nullptr; }
  explicit Joint(const char* n) : Joint() { name = n; }
  ~Joint() {
    if (box) delete box;

    // children
    for (Joint* j : children) {
      delete j;
    }
  }

  void AddChild(Joint* ch);
  bool Load(Tokenizer& t);
  void Update(const glm::mat4& parent);
  void Draw(const glm::mat4& viewProjMtx, GLuint shader);
  void SetPosition(glm::vec3 pos);
  void SetDOF(int dof, float v);
  void ShowAll();  // GUI
  void Show();     // GUI, self

  glm::mat4 GetWorldMatrix();
  Joint* GetChild(int index);
  Joint* GetChildReversed(int index);
  glm::vec3 GetPosition();
  std::vector<DOF> GetDOFS();

 private:
  std::string name = "bone";

  glm::vec3 offset;
  std::vector<DOF> phi = {DOF(), DOF(), DOF()};
  glm::mat4 ComputeLocal();

  glm::mat4 worldMatrix;
  Cube* box;

  std::vector<Joint*> children;
};

////////////////////////////////////////////////////////////////////////////////
