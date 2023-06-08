////////////////////////////////////////
// DOF.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include <imgui.h>
#include <stdlib.h>

#include <sstream>
#include <vector>

#include "client/graphics/core.h"

using glm::vec3;

////////////////////////////////////////////////////////////////////////////////

// The DOF struct, holds info for gui-editable values.

struct DOF {
  float value = 0.0;
  float min = -100000.0;
  float max = 100000.0;

  DOF() {}
  explicit DOF(float v) { SetMinMax(v, v); }
  DOF(float lo, float hi) { SetMinMax(lo, hi); }
  DOF(float lo, float hi, float val) : DOF(lo, hi) { SetValue(val); }

  void Reset(void) { SetValue(0.0); }

  void SetValue(const float v) { value = glm::clamp(v, min, max); }

  const float GetValue(void) { return value; }

  void SetMinMax(const float lo, const float hi) {
    min = lo;
    max = hi;

    SetValue(value);
  }

  virtual void Show(const char* name, const char* group) {
    if (max - min > 500)
      ImGui::DragFloat(name, &value, 0.1f, min, max);  // bigger ranges
    else
      ImGui::SliderFloat(name, &value, min, max);
  }
};

// Alternative DOF for random ranges
struct DOFr : public DOF {
  float initMin;
  float initMax;
  bool random = true;

  DOFr(float initlo, float inithi) : DOF(initlo, inithi) {
    initMin = initlo;
    initMax = inithi;
  }

  DOFr(float initlo, float inithi, float v) : DOF(v, v) {
    initMin = initlo;
    initMax = inithi;
  }

  explicit DOFr(float v) { DOFr(v, v, v); }

  const float GetRandomValue(void) { return Tools::random(min, max); }

  void SetMin(const float lo) {
    min = lo;

    SetValue(value);
  }
  const float GetMin(void) { return min; }

  void SetMax(const float hi) {
    max = hi;

    SetValue(value);
  }
  const float GetMax(void) { return max; }

  static glm::vec3 GetRandomVector3(std::vector<DOFr> v) {
    return vec3(v[0].GetRandomValue(), v[1].GetRandomValue(),
                v[2].GetRandomValue());
  }

  void Show(const char* name, const char* group) override {
    DOF::Show(name, group);
  }
};

struct DOF3 {
  std::vector<DOF> phi = {DOF(), DOF(), DOF()};

  DOF3() {}
  DOF3(DOF x, DOF y, DOF z) { phi = {x, y, z}; }

  float x() { return phi[0].GetValue(); }
  float y() { return phi[1].GetValue(); }
  float z() { return phi[2].GetValue(); }

  void show(std::string name) {
    std::ostringstream def;
    def << name << ".X";
    std::string defstrX = def.str();

    std::ostringstream def2;
    def2 << name << ".Y";
    std::string defstrY = def2.str();

    std::ostringstream def3;
    def3 << name << ".Z";
    std::string defstrZ = def3.str();

    // ImGui::SameLine();
    phi[0].Show(defstrX.c_str(), name.c_str());

    phi[1].Show(defstrY.c_str(), name.c_str());

    phi[2].Show(defstrZ.c_str(), name.c_str());
  }
};

////////////////////////////////////////////////////////////////////////////////
