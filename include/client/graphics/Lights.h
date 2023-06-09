#pragma once

#include <imgui.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "client/graphics/DOF.h"
#include "client/graphics/core.h"

namespace SettingsStructs {

using glm::vec3;
using glm::vec4;

struct Light {
  DOF3 direction = DOF3(DOF(-1, 1, 0), DOF(-1, 1, 0), DOF(-1, 1, 0));
  DOF3 color = DOF3(DOF(0, 1, 0), DOF(0, 1, 0), DOF(0, 1, 0));

  void show(std::string name) {
    direction.show("light_" + name + "_dir");
    color.show("light_" + name + "_col");
  }
};

// Simple light data to be passed to shaders.
struct Lights {
  bool enableLights = false;

  std::vector<Light> lights;

  Lights() {
    for (unsigned int i = 0; i < 2; i++) {
      lights.push_back(Light());
    }
  }

  std::vector<vec3> GetLightDirections() {
    std::vector<vec3> r;

    for (auto li : lights) {
      r.push_back(vec3(li.direction.x(), li.direction.y(), li.direction.z()));
    }

    return r;
  }

  std::vector<vec3> GetLightColors() {
    std::vector<vec3> r;

    for (auto li : lights) {
      r.push_back(vec3(li.color.x(), li.color.y(), li.color.z()));
    }

    return r;
  }

  void gui() {
    ImGui::Begin("lighting +++ ");

    ImGui::Checkbox("enable lights", &enableLights);

    ImGui::Separator();

    for (unsigned int i = 0; i < lights.size(); i++) {
      lights[i].show(std::to_string(i));
    }

    ImGui::End();
  }
};
}  // namespace SettingsStructs
