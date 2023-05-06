#ifndef _CORE_H_
#define _CORE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <ctype.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <vector>

inline float modu(float x, float y) { return x - y * floor(x / y); }

template <class T>
struct optional {
  T val;
  bool exists;
  optional() : exists(false) {}
  optional(const T& t) : val(t), exists(true) {}
  ~optional() {
    if (exists) val.~T();
  }
};

#endif