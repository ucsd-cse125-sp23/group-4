////////////////////////////////////////
// AnimationPlayer.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include "AnimationClip.h"
#include "core.h"

////////////////////////////////////////////////////////////////////////////////

// The AnimationPlayer class for playing animation clips.

class AnimationPlayer {
 public:
  float speed;

  AnimationPlayer() {
    time = 0;
    clip = nullptr;
  }
  ~AnimationPlayer() {}

  void SetClip(AnimationClip* c) { clip = c; }

  const Pose GetCurrentPose() { return pose; }

  void Update(float deltaTime) {
    if (clip) {
      clip->Evaluate(time, pose);
    }

    time += deltaTime * speed;
  }

  void Play() { time = 0.0f; }

 private:
  float time;
  AnimationClip* clip;
  Pose pose;
};

////////////////////////////////////////////////////////////////////////////////
