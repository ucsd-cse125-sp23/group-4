////////////////////////////////////////
// Channel.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include "Keyframe.h"
#include "Tokenizer.h"

////////////////////////////////////////////////////////////////////////////////

// The Channel struct, holds info for individual values.

enum ExtrapolateType { constant, linear, cycle, cycle_offset, bounce };

struct Channel {
  ExtrapolateType extrapolateIn = constant;
  ExtrapolateType extrapolateOut = constant;

  std::vector<Keyframe> keys;
  int lastKey = 0;

  Channel() {}
  Channel(int numkeys, ExtrapolateType ein, ExtrapolateType eout) {
    keys.reserve(numkeys);

    extrapolateIn = ein;
    extrapolateOut = eout;
  }

  bool Load(Tokenizer& t) {
    t.FindToken("{");

    t.FindToken("extrapolate");
    extrapolateIn = GetExtrapolation(t);   // extrap_in
    extrapolateOut = GetExtrapolation(t);  // extrap_out

    t.FindToken("keys");
    int numkeys = t.GetInt();
    keys.reserve(numkeys);
    t.FindToken("{");
    for (int i = 0; i < numkeys; i++) {
      float ti = t.GetFloat();
      float va = t.GetFloat();

      std::pair<TangentRule, float> tanIn = GetTangentRule(t);  // tangent_in

      std::pair<TangentRule, float> tanOu = GetTangentRule(t);  // tangent_out

      Keyframe k = Keyframe(ti, va, tanIn.first, tanOu.first);
      k.SetTangents(tanIn.second, tanOu.second);

      keys.push_back(k);
    }
    t.FindToken("}");
    keys.resize(numkeys);

    t.FindToken("}");
    return true;
  }

  void Precompute() {
    int max = keys.size();

    // Compute tangents pass
    for (int i = 0; i < max; i++) {
      int p = i - 1;
      int n = i + 1;

      optional<Keyframe> prev = (p >= 0) ? keys[p] : optional<Keyframe>();
      optional<Keyframe> next = (n < max) ? keys[n] : optional<Keyframe>();

      keys[i].ComputeTangents(prev, next);
    }

    // Compute cubic coefficients pass
    for (int i = 0; i < max; i++) {
      int n = i + 1;

      if (n >= max) continue;

      keys[i].ComputeCoefficients(keys[n]);
    }
  }

  float Evaluate(float t) {
    if (keys.size() <= 0) return 0.0f;
    if (keys.size() == 1) return keys[0].value;

    int end = keys.size() - 1;

    Keyframe k0 = keys[0];
    Keyframe ke = keys[end];

    if (t < k0.time || t > ke.time) {
      // Extrapolate

      float r = ke.time - k0.time;
      float v = k0.time + modu(t - ke.time, r);
      float h = ke.value - k0.value;

      bool alter = modu(t - ke.time, 2 * r) < r;

      if (t < k0.time) {
        // Extrapolate in

        switch (extrapolateIn) {
          case constant:
            return k0.value;
            break;
          case linear:
            return k0.tangentIn * (t - k0.time) + k0.value;
            break;
          case cycle:
            t = v;
            break;
          case cycle_offset:
            return Evaluate(v) + h * ceilf((t - ke.time) / r);
            break;
          case bounce:
            t = alter ? ke.time - v + k0.time : v;
            break;
          default:
            return k0.value;
            break;
        }
      } else if (t > ke.time) {
        // Extrapolate out

        switch (extrapolateOut) {
          case constant:
            return ke.value;
            break;
          case linear:
            return ke.tangentOut * (t - ke.time) + ke.value;
            break;
          case cycle:
            t = v;
            break;
          case cycle_offset:
            return Evaluate(v) + h * ceilf((t - ke.time) / r);
            break;
          case bounce:
            t = alter ? ke.time - v + k0.time : v;
            break;
          default:
            return ke.value;
            break;
        }
      }
    }

    int i = Search(keys, lastKey, lastKey + 1, t);

    if (i < 0) i = Search(keys, 0, end, t);
    if (i < 0) return 0.0f;

    Keyframe l = keys[i];

    if (i + 1 > end) return l.Evaluate(0.0f);  // in case there's no next key

    Keyframe r = keys[i + 1];

    float u = (t - l.time) / (r.time - l.time);  // InvLerp

    return l.Evaluate(u);
  }

  static int Search(std::vector<Keyframe> arr, int l, int r, float t) {
    // Binary search through keyframe times
    Keyframe lk = arr[l];
    Keyframe rk = arr[r];

    while (l <= r && lk.time <= rk.time) {
      int m = l + (r - l) / 2;

      Keyframe mk = arr[m];

      // If t is in this span
      if (mk.time == t || (mk.time < t && arr[m + 1].time > t)) return m;

      if (arr[m + 1].time <= t)  // If t is greater than this span
        l = m + 1;

      else  // If t isn't greater than this span, it must be less than this span
        r = m - 1;

      lk = arr[l];
      if (r < 0) return 0;
      rk = arr[r];
    }

    return -1;
  }

  // "constant", "linear", "cycle", "cycle_offset", or "bounce".
  static ExtrapolateType GetExtrapolation(Tokenizer& t) {
    char temp[256];
    t.GetToken(temp);  // tangent value

    if (strcmp(temp, "constant") == 0) {
      return constant;
    } else if (strcmp(temp, "linear") == 0) {
      return linear;
    } else if (strcmp(temp, "cycle") == 0) {
      return cycle;
    } else if (strcmp(temp, "cycle_offset") == 0) {
      return cycle_offset;
    } else if (strcmp(temp, "bounce") == 0) {
      return bounce;
    }
  }

  // "flat", "linear", "smooth", or a floating point slope value indicating the
  // fixed tangent mode.
  static std::pair<TangentRule, float> GetTangentRule(Tokenizer& t) {
    char temp[256];
    t.GetToken(temp);  // tangent value

    if (strcmp(temp, "flat") == 0) {
      return std::make_pair(TangentRule::flat, 0.0f);
    } else if (strcmp(temp, "linear") == 0) {
      return std::make_pair(TangentRule::line, 0.0f);
    } else if (strcmp(temp, "smooth") == 0) {
      return std::make_pair(TangentRule::smooth, 0.0f);
    }

    // Handle fixed float case
    return std::make_pair(TangentRule::fixed, float(atof(temp)));
  }
};

////////////////////////////////////////////////////////////////////////////////
