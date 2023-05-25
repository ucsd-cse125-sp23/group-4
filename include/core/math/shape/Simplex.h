#pragma once

#include <algorithm>
#include <array>
#include <initializer_list>

#include "core/math/vector.h"


struct Simplex {
 private:
  std::array<vec3f, 4> pts;
  size_t _size;

 public:
  Simplex()
      : pts({vec3f(0, 0, 0), vec3f(0, 0, 0), vec3f(0, 0, 0), vec3f(0, 0, 0)}),
        _size(0) {}
  Simplex& operator=(std::initializer_list<vec3f> list) {
    for (auto v = list.begin(); v != list.end(); v++) {
      pts[std::distance(list.begin(), v)] = *v;
    }
    _size = list.size();

    return *this;
  }

  void push(vec3f pt) {
    pts = {pt, pts[0], pts[1], pts[2]};
    _size = std::min(_size + 1, static_cast<std::size_t>(4));
  }

  vec3f& operator[](unsigned int i) { return this->pts[i]; }
  unsigned int size() const { return this->_size; }

  auto begin() const { return pts.begin(); }
  auto end() const { return pts.end() - (4 - _size); }
};
