#pragma once

#include <imgui.h>

#include "Lights.h"
#include "Music.h"
#include "SoundEffect.h"
// Runtime variables. For client only.
struct Settings {
  float gammaCorrection = 1.5f;
  // float volume;   // controlled via SoundEffect static var

  SettingsStructs::Lights lightConfig = SettingsStructs::Lights();

  void gui(float* fov) {
    ImGui::Begin("game settings +++");
    ImGui::SliderFloat("FOV", fov, 30.0f, 200.0f);
    ImGui::DragFloat("gamma", &gammaCorrection, 0.1f, 0.01f, 5.0f);
    ImGui::Separator();
    ImGui::SliderFloat("sfx  volume", &SoundEffect::volumeGlobal, 0.0f, 4.0f);
    ImGui::Separator();
    ImGui::SliderFloat("song volume", &Music::volumeGlobal, 0.0f, 4.0f);
    ImGui::End();

    lightConfig.gui();
  }
};

extern Settings settings;
