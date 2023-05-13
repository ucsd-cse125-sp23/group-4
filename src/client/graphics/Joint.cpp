#include "Joint.h"

#include <string.h>

#include <ostream>

#include "glm/gtx/transform.hpp"

void Joint::AddChild(Joint* child) { children.push_back(child); }

// Ljoint(phi)
glm::mat4 Joint::ComputeLocal() {
  // right terms first, left term last
  return glm::translate(offset) *
         glm::rotate(phi[2].GetValue(), glm::vec3(0.0f, 0.0f, 1.0f)) *
         glm::rotate(phi[1].GetValue(), glm::vec3(0.0f, 1.0f, 0.0f)) *
         glm::rotate(phi[0].GetValue(), glm::vec3(1.0f, 0.0f, 0.0f));
}

/////////////////////////
// Format:
// offset           x y z
// boxmin           x y z
// boxmax           x y z
// rotxlimit        min max
// rotylimit        min max
// rotzlimit        min max
// pose             x y z
// balljoint        name { }
//
/////////////////////////
bool Joint::Load(Tokenizer& t) {
  glm::vec3 bmin = glm::vec3(-0.1);
  glm::vec3 bmax = glm::vec3(0.1);

  t.FindToken("{");
  while (1) {
    char temp[256];
    t.GetToken(temp);
    if (strcmp(temp, "offset") == 0) {
      offset.x = t.GetFloat();
      offset.y = t.GetFloat();
      offset.z = t.GetFloat();
    } else if (strcmp(temp, "boxmin") == 0) {
      bmin.x = t.GetFloat();
      bmin.y = t.GetFloat();
      bmin.z = t.GetFloat();
    } else if (strcmp(temp, "boxmax") == 0) {
      bmax.x = t.GetFloat();
      bmax.y = t.GetFloat();
      bmax.z = t.GetFloat();
    } else if (strcmp(temp, "rotxlimit") == 0) {
      float l = t.GetFloat();
      float r = t.GetFloat();
      phi[0].SetMinMax(l, r);
    } else if (strcmp(temp, "rotylimit") == 0) {
      float l = t.GetFloat();
      float r = t.GetFloat();
      phi[1].SetMinMax(l, r);
    } else if (strcmp(temp, "rotzlimit") == 0) {
      float l = t.GetFloat();
      float r = t.GetFloat();
      phi[2].SetMinMax(l, r);
    } else if (strcmp(temp, "pose") == 0) {
      phi[0].SetValue(t.GetFloat());
      phi[1].SetValue(t.GetFloat());
      phi[2].SetValue(t.GetFloat());
    } else if (strcmp(temp, "balljoint") == 0) {
      char tempname[256];
      t.GetToken(tempname);  // name
      Joint* jnt = new Joint(&tempname[0]);
      jnt->Load(t);
      AddChild(jnt);
    } else if (strcmp(temp, "}") == 0) {
      break;
    } else {
      t.SkipLine();  // Unrecognized token
    }
  }

  // box = new Cube(bmin, bmax);

  return true;
}

void Joint::Update(const glm::mat4& parent) {
  glm::mat4 local = ComputeLocal();
  glm::mat4 world = parent * local;

  worldMatrix = world;
  // box->setMatrix(world);

  // children
  for (Joint* j : children) {
    j->Update(world);
  }
}

void Joint::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
  // box->draw(viewProjMtx, shader);

  // children
  for (Joint* j : children) {
    j->Draw(viewProjMtx, shader);
  }
}

void Joint::SetPosition(glm::vec3 pos) { offset = pos; }

void Joint::SetDOF(int dof, float v) { phi[dof].SetValue(v); }

void Joint::ShowAll() {
  Show();

  // children
  for (Joint* j : children) {
    j->ShowAll();
  }
}

void Joint::Show() {
  std::ostringstream def;
  def << name << ".RotX";
  std::string defstrX = def.str();

  std::ostringstream def2;
  def2 << name << ".RotY";
  std::string defstrY = def2.str();

  std::ostringstream def3;
  def3 << name << ".RotZ";
  std::string defstrZ = def3.str();

  // ImGui::SameLine();
  phi[0].Show(defstrX.c_str(), name.c_str());

  phi[1].Show(defstrY.c_str(), name.c_str());

  phi[2].Show(defstrZ.c_str(), name.c_str());
}

// Getters

glm::mat4 Joint::GetWorldMatrix() { return worldMatrix; }

Joint* Joint::GetChild(int child) {
  if (child < 0 || child >= children.size()) return nullptr;

  return children[child];
}

Joint* Joint::GetChildReversed(int child) {
  // Treats the last joint of the children array as the beginning
  // Used for DFS, allows correct joint order
  if (child < 0 || child >= children.size()) return nullptr;

  return children[children.size() - (child + 1)];
}

glm::vec3 Joint::GetPosition() { return offset; }

std::vector<DOF> Joint::GetDOFS() { return phi; }
