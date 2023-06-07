/**************************************************
 * SoundEffect.h
 * representation of a sound to be stored in memory
 *****************************************************/

#pragma once

#include <SFML/Audio.hpp>
#include <string>

class SoundEffect {
 public:
  inline static float volumeGlobal = 1.0f;  // 0.0 to 1.0

  void load(std::string filename) {
    sound = loadSound(filename);
    setEffectVolume(1.0f);
  }

  void setEffectVolume() { sound.setVolume(100 * volume * volumeGlobal); }
  void setEffectVolume(float v) {
    volume = v;
    setEffectVolume();
  }

  void play() {
    setEffectVolume();
    sound.play();
  }

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

    return sf::Sound(buf);
  }
};
