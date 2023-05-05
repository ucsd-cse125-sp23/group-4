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

  void Reset(void) { SetValue(0.0); }

  void SetValue(const float v) { value = glm::clamp(v, min, max); }

  const float GetValue(void) { return value; }

  void SetMinMax(const float lo, const float hi) {
    min = lo;
    max = hi;

    SetValue(value);
  }

  void Show(const char* name, const char* group) {
    if (max - min > 500)
      ImGui::DragFloat(name, &value, 0.1f, min, max);  // bigger ranges
    else
      ImGui::SliderFloat(name, &value, min, max);
  }
};

////////////////////////////////////////////////////////////////////////////////
