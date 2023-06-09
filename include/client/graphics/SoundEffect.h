/**************************************************
 * SoundEffect.h
 * representation of a sound to be stored in memory
 *****************************************************/

#pragma once

#include <SFML/Audio.hpp>
#include <string>

#include "client/graphics/core.h"
class SoundEffect {
 public:
  inline static float volumeGlobal = 1.9f;  // 0.0 to 1.0

  void load(std::string filename) {
    sound = loadSound(filename);
    setEffectVolume(1.0f);
  }

  void setEffectVolume() { sound.setVolume(100 * volume * volumeGlobal); }
  void setEffectVolume(float v) {
    volume = v;
    setEffectVolume();
  }

  void play(glm::vec3 pos) {
    sound.setPosition(pos.x, pos.y, pos.z);
    setEffectVolume();
    if (sound.getStatus() != sf::Sound::Status::Playing) {
      sound.play();
    }
  }
  void loop() { sound.setLoop(true); }

  void stop() { sound.stop(); }

 private:
  float volume = 1.0f;

  sf::SoundBuffer buf;
  sf::Sound sound;

  sf::Sound loadSound(std::string filename) {
    bool success = buf.loadFromFile(filename);

    if (!success) {
      printf(("SoundEffect: Failed to load " + filename + ".\n").c_str());
      return sf::Sound(buf);
    }
    sound.setRelativeToListener(false);
    sound.setAttenuation(40.0f);
    return sf::Sound(buf);
  }
};
