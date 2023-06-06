#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <ctype.h>
#include <math.h>
#include <time.h>

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Tools {
 public:
  static void inittools() { srand(static_cast<unsigned>(time(0))); }

  static glm::vec3 randomizeSphere() {
    float r =
        static_cast<float>(rand()) / static_cast<float>(RAND_MAX);  // NOLINT
    float q =
        static_cast<float>(rand()) / static_cast<float>(RAND_MAX);  // NOLINT

    float u = 2 * glm::pi<float>() * r;
    float v = glm::sqrt(q * (1 - q));

    float x = glm::dot(2 * v, cos(u));
    float y = 1 - 2 * q;
    float z = glm::dot(2 * v, sin(u));

    return glm::vec3(x, y, z);
  }

  static float random(float l, float h) {
    float r = l + static_cast<float>(rand()) /
                      static_cast<float>(RAND_MAX / (h - l));  // NOLINT

    return r;
  }
};

struct DrawInfo {
  glm::mat4 viewProjMtx;
  glm::mat4 viewProjOrigMtx;
  glm::mat4 viewMtx;

  const glm::mat4 GetVP(bool origin) {
    return origin ? viewProjOrigMtx : viewProjMtx;
  }

  /**DrawInfo(const glm::mat4& vp, const glm::mat4& v, GLuint sdr) :
  viewProjMtx(vp), viewMtx(v), shader(sdr) {

  }*/
};

inline float modu(float x, float y) { return x - y * floor(x / y); }

inline glm::vec3 unit(glm::vec3 v) {
  if (glm::length(v) == 0) return glm::vec3(0);  // avoid NaN
  return glm::normalize(v);
}

inline glm::vec3 clampMagnitude(glm::vec3 v, float max) {
  return unit(v) * std::min(glm::length(v), max);
}

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
