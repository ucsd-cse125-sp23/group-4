#pragma once
#include <string>

class Timer {
 public:
  float time;
  bool countdown;
  Timer() {
    time = 0.0f;
    countdown = false;
  }

  void Update(float deltaTime) {
    if (!countdown) {
      time += deltaTime;
    } else {
      time -= deltaTime;
      if (time < 0) {
        time = 0.0f;
      }
    }
  }

  std::string ToString() {
    int minutes = floor(time / 60);
    int seconds = (time >= 1) ? (static_cast<int>(time) % 60) : 0;
    return std::to_string(minutes) + "m" + std::to_string(seconds) + "s";
  }
};
