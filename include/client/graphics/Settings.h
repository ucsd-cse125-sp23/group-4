#pragma once

#include <imgui.h>

#include "Lights.h"
#include "SoundEffect.h"

// Runtime variables. For client only.
struct Settings {
  float gammaCorrection = 1.5f;
  // float volume;   // controlled via SoundEffect static var

  SettingsStructs::Lights lightConfig = SettingsStructs::Lights();

  void gui() {
    ImGui::Begin("game settings +++");

    ImGui::DragFloat("volume", &SoundEffect::volumeGlobal, 0.2f, 0.0f, 1.5f);

    ImGui::Separator();

    ImGui::DragFloat("gamma", &gammaCorrection, 0.2f, 0.01f, 5.0f);

    ImGui::End();

    lightConfig.gui();
  }
};

extern Settings settings;
