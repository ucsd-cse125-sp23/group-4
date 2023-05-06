////////////////////////////////////////
// Keyframe.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include "core.h"
using namespace ::glm;

////////////////////////////////////////////////////////////////////////////////

// The Keyframe struct, holds info for a single key within a channel and handles
// local computations.

enum class TangentRule { flat, line, smooth, fixed };

struct Keyframe {
  float time = 0;
  float value = 0;

  float tangentIn, tangentOut;
  TangentRule ruleIn, ruleOut;  // tangent rules
  float a, b, c, d;             // cubic coefficients

  Keyframe() {
    tangentIn = 0, tangentOut = 0;
    ruleIn = TangentRule::fixed, ruleOut = TangentRule::fixed;
    a = 0;
    b = 0;
    c = 0;
    d = 0;
  }
  Keyframe(float t, float v) : Keyframe() {
    time = t;
    value = v;
  }
  Keyframe(float t, float v, TangentRule in, TangentRule out) : Keyframe(t, v) {
    ruleIn = in;
    ruleOut = out;
  }

  void SetTangents(float in, float out) {
    tangentIn = in;
    tangentOut = out;
  }

  void ComputeTangents(optional<Keyframe> prevOpt, optional<Keyframe> nextOpt) {
    Keyframe prev = prevOpt.val;
    Keyframe next = nextOpt.val;

    bool inCopyOut = false;

    switch (ruleIn) {
      case TangentRule::flat:
        tangentIn = 0.0f;
        break;
      case TangentRule::line:
        if (!prevOpt.exists)
          inCopyOut = true;
        else
          tangentIn = (value - prev.value) / (time - prev.time);
        break;
      case TangentRule::smooth:
        if (!prevOpt.exists)
          inCopyOut = true;
        else if (!nextOpt.exists)
          tangentIn = (value - prev.value) / (time - prev.time);
        else
          tangentIn = (next.value - prev.value) / (next.time - prev.time);
        break;
      default:
        // Fixed: should already be set from "SetTangents"
        break;
    }

    switch (ruleOut) {
      case TangentRule::flat:
        tangentOut = 0.0f;
        break;
      case TangentRule::line:
        if (!nextOpt.exists)
          tangentOut = tangentIn;
        else
          tangentOut = (next.value - value) / (next.time - time);
        break;
      case TangentRule::smooth:
        if (!nextOpt.exists)
          tangentOut = tangentIn;
        else if (!prevOpt.exists)
          tangentOut = (next.value - value) / (next.time - time);
        else
          tangentOut = (next.value - prev.value) / (next.time - prev.time);
        break;
      default:
        break;
    }

    if (inCopyOut) tangentIn = tangentOut;
  }

  /*
   * a   2 -2  1  1     p0
   * b   -3 3 -2 -1     p1
   * c = 0  0  1  0  *  t1-t0 v0
   * d   1  0  0  0     t1-t0 v1
   */
  void ComputeCoefficients(Keyframe next) {
    mat4 l = mat4(1.0f);
    // columns
    l[0] = glm::vec4(2, -3, 0, 1);
    l[1] = glm::vec4(-2, 3, 0, 0);
    l[2] = glm::vec4(1, -2, 1, 0);
    l[3] = glm::vec4(1, -1, 0, 0);

    vec4 r = vec4(value, next.value, (next.time - time) * tangentOut,
                  (next.time - time) * next.tangentIn);

    vec4 abcd = l * r;
    a = abcd.x;
    b = abcd.y;
    c = abcd.z;
    d = abcd.w;
  }

  float Evaluate(float u) { return d + u * (c + u * (b + u * a)); }
};

////////////////////////////////////////////////////////////////////////////////
