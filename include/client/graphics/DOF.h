////////////////////////////////////////
// DOF.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include <imgui.h>
#include <stdlib.h>

#include <sstream>

////////////////////////////////////////////////////////////////////////////////

// The DOF struct, holds info for posing joints.

struct DOF {
  float value = 0.0;
  float min = -100000.0;
  float max = 100000.0;

  DOF() {}
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

////////////////////////////////////////////////////////////////////////////////
