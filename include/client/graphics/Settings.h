#pragma once

#include <imgui.h>

#include "SoundEffect.h"
#include "Music.h"
// Runtime variables. For client only.
struct Settings {
  float gammaCorrection = 1.5f;
  // float volume = 0.5f;   // controlled via SoundEffect static var

  void gui() {
    ImGui::Begin("game settings +++");

    ImGui::DragFloat("gamma", &gammaCorrection, 0.2f, 0.01f, 5.0f);
    ImGui::DragFloat("sfx  volume", &SoundEffect::volumeGlobal, 0.2f, 0.0f, 1.5f);
    ImGui::DragFloat("song volume", &Music::volumeGlobal, 0.2f, 0.0f, 1.5f);
    ImGui::End();
  }
};

extern Settings settings;
