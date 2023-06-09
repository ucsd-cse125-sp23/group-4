#include <SFML/Audio.hpp>
#include <string>
#pragma once 
class Music {
 public:
  inline static float volumeGlobal = 0.2f;  // 0.0 to 1.0

  void load(std::string filename) {
    if (!loadSound(filename)) {
      std::cerr << "failed " << std::endl;
    }
  }

  void setEffectVolume() { music.setVolume(20 * volume * volumeGlobal); }
  void setEffectVolume(float v) {
    volume = v;
    setEffectVolume();
  }

  void play() {
    setEffectVolume();
    std::cerr << "please be called \n\n\n\n" << std::endl;
    music.play();
  }
  void loop() { music.setLoop(true); }

  void stop() { music.stop(); }

 private:
  float volume = .05f;
  // sf::Sound sound;
  sf::Music music;

  bool loadSound(std::string filename) {
    music.setRelativeToListener(true);
    music.setLoop(true);
    return music.openFromFile(filename);
  }
};