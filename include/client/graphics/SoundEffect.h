/**************************************************
 * SoundEffect.h
 * representation of a sound to be stored in memory
 *****************************************************/

#pragma once

#include <SFML/Audio.hpp>
#include <string>

class SoundEffect {
 public:
  void load(std::string filename) {
    sound = loadSound(filename);
    sound.setVolume(100);
  }

  void play() { sound.play(); }

 private:
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
