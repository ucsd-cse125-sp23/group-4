#pragma once

#include <imgui.h>

// Runtime variables. For client only.
struct Settings {
  float gammaCorrection = 1.5f;
  float volume = 0.5f;

  void gui() {
    ImGui::Begin("game settings +++");

    ImGui::DragFloat("gamma", &gammaCorrection, 0.2f, 0.01f, 5.0f);
    ImGui::DragFloat("volume", &volume, 0.2f, 0.0f, 1.5f);

    ImGui::End();
  }
};

extern Settings settings;
