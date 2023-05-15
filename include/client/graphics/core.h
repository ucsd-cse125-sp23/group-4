#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <ctype.h>
#include <math.h>

inline float modu(float x, float y) { return x - y * floor(x / y); }

template <class T>
struct optional {
  T val;
  bool exists;
  optional() : exists(false) {}
  explicit optional(const T& t) : val(t), exists(true) {}
  ~optional() {
    if (exists) val.~T();
  }
};
