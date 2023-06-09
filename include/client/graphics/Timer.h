#pragma once
#include <string>

class Timer {
 public:
  float time;
  Timer() { time = 0.0f; }

  void Update(float seconds_remaining) { time = seconds_remaining; }

  std::string ToString() {
    int minutes = floor(time / 60);
    int seconds = (time >= 1) ? (static_cast<int>(time) % 60) : 0;
    return std::to_string(minutes) + "m" + std::to_string(seconds) + "s";
  }
};
