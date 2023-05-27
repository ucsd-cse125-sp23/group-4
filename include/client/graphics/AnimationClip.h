////////////////////////////////////////
// AnimationClip.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include <vector>

#include "client/graphics/Channel.h"
#include "client/graphics/Pose.h"
#include "client/graphics/imported/Tokenizer.h"

////////////////////////////////////////////////////////////////////////////////

// The AnimationClip class for storing a single animation's channel data and
// coordinating them.

class AnimationClip {
 public:
  AnimationClip() {
    timeStart = 0, timeEnd = 0;

    currTime = 0;
  }
  ~AnimationClip() {}

  bool Load(const char* file);
  void Show();  // GUI

  void Evaluate(float time, Pose& pose);

 private:
  float timeStart, timeEnd;
  bool cycleTime = true;
  float currTime;  // Can be locked to start/end range

  std::vector<Channel> channels;
};

////////////////////////////////////////////////////////////////////////////////
