#include "AnimationClip.h"

bool AnimationClip::Load(const char* file) {
  Tokenizer t;
  bool success = t.Open(file);

  if (!success) return false;

  t.FindToken("animation");
  t.FindToken("{");

  t.FindToken("range");
  timeStart = t.GetFloat();
  timeEnd = t.GetFloat();

  t.FindToken("numchannels");
  int numchannels = t.GetInt();
  channels.reserve(numchannels);

  for (int i = 0; i < numchannels; i++) {
    t.FindToken("channel");
    int chindex = t.GetInt();

    Channel c = Channel();
    c.Load(t);  // Load channel data in

    c.Precompute();  // Precompute channel

    channels.push_back(c);
  }

  t.FindToken("}");

  channels.resize(numchannels);

  // Finish
  t.Close();

  return true;
}

void AnimationClip::Show() {
  ImGui::Checkbox("Cycle Mode", &cycleTime);
  ImGui::Separator();
  ImGui::Text("Time = %f", currTime);
}

void AnimationClip::Evaluate(float time, Pose& pose) {
  currTime = time;

  if (cycleTime) {
    // Cycle entire clip within time range
    float r = timeEnd - timeStart;
    currTime = timeStart + modu(time - timeEnd, r);
  }

  if (pose.size() != channels.size()) pose.resize(channels.size());

  for (int i = 0; i < pose.size(); i++) {
    pose[i] = channels[i].Evaluate(currTime);
  }
}
