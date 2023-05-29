#pragma once

#include "core/math/shape/ConvexShape.h"

#include <limits>
#include <vector>
#include <algorithm>

#include "core/math/shape/LineShape.h"
#include "core/math/shape/PointShape.h"
#include "core/math/shape/Simplex.h"

#define MAX_ITERATIONS 100
#define TOLERANCE 0.001

// https://blog.winter.dev/2020/gjk-algorithm/
// https://blog.winter.dev/2020/epa-algorithm/
// https://stackoverflow.com/questions/48979868/implementing-the-expanding-polytope-algorithm-in-3d-space
vec3f ConvexShape::support(const ConvexShape* shape0, const mat4f& mtx0,
                           const mat3f& iMtx0, const ConvexShape* shape1,
                           const mat4f& mtx1, const mat3f& iMtx1, vec3f dir) {
  return vec3f(mtx0 * shape0->furthestPoint(iMtx0 * dir) -
               mtx1 * shape1->furthestPoint(iMtx1 * (-dir)));
}

bool sameDir(const vec3f& dir, const vec3f& ao) { return dot(dir, ao) > 0; }
bool line(Simplex& pts, vec3f& dir) {
  vec3f a = pts[0];
  vec3f b = pts[1];

  vec3f ab = b - a;
  vec3f ao = -a;

  if (sameDir(ab, ao)) {
    dir = cross(cross(ab, ao), ab);
    if (length_squared(dir) == 0) return true;
  } else {
    pts = {a};
    dir = ao;
  }

  return false;
}
bool triangle(Simplex& pts, vec3f& dir) {
  vec3f a = pts[0];
  vec3f b = pts[1];
  vec3f c = pts[2];

  vec3f ab = b - a;
  vec3f ac = c - a;
  vec3f ao = -a;

  vec3f abc = cross(ab, ac);

  if (sameDir(cross(abc, ac), ao)) {
    if (sameDir(ac, ao)) {
      pts = {a, c};
      dir = cross(cross(ac, ao), ac);
    }

    else {
      return line(pts = {a, b}, dir);
    }
  }

  else {
    if (sameDir(cross(ab, abc), ao)) {
      return line(pts = {a, b}, dir);
    }

    else {
      if (sameDir(abc, ao)) {
        dir = abc;
      }

      else {
        pts = {a, c, b};
        dir = -abc;
      }
    }
  }

  return false;
}
bool tetrahedron(Simplex& pts, vec3f& dir) {
  vec3f a = pts[0];
  vec3f b = pts[1];
  vec3f c = pts[2];
  vec3f d = pts[3];

  vec3f ab = b - a;
  vec3f ac = c - a;
  vec3f ad = d - a;
  vec3f ao = -a;

  vec3f abc = cross(ab, ac);
  vec3f acd = cross(ac, ad);
  vec3f adb = cross(ad, ab);

  if (sameDir(abc, ao)) {
    return triangle(pts = {a, b, c}, dir);
  }

  if (sameDir(acd, ao)) {
    return triangle(pts = {a, c, d}, dir);
  }

  if (sameDir(adb, ao)) {
    return triangle(pts = {a, d, b}, dir);
  }

  return true;
}
bool nextSimplex(Simplex& pts, vec3f& direction) {
  switch (pts.size()) {
    case 2:
      return line(pts, direction);
    case 3:
      return triangle(pts, direction);
    case 4:
      return tetrahedron(pts, direction);
  }
  return false;
}

bool ConvexShape::collides(const ConvexShape* other, const mat4f& thisMtx,
                           const mat3f& thisIMtx, const mat4f& otherMtx,
                           const mat3f& otherIMtx) const {
  vec3f support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                       otherIMtx, {1, 0, 0});
  if (length_squared(support) == 0) return true;
  Simplex pts;
  pts.push(support);

  size_t ite = 0;
  vec3f dir = -support;
  while (ite++ < MAX_ITERATIONS) {
    support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                   otherIMtx, dir);
    if (dot(support, dir) <= 0) return false;
    pts.push(support);
    if (nextSimplex(pts, dir)) return true;
  }
  return false;
}
bool ConvexShape::collides(const ConvexShape* other, const mat4f& thisMtx,
                           const mat4f& otherMtx) const {
  mat3f thisIMtx = mat3f(thisMtx);
  if (thisIMtx != mat3f::identity()) thisIMtx = inverse(thisIMtx);
  mat3f otherIMtx = mat3f(otherMtx);
  if (otherIMtx != mat3f::identity()) otherIMtx = inverse(otherIMtx);

  return this->collides(other, thisMtx, thisIMtx, otherMtx, otherIMtx);
}
bool ConvexShape::collides(const BoundingShape* other, const mat4f& thisMtx,
                           const mat4f& otherMtx) const {
  mat3f thisIMtx = mat3f(thisMtx);
  if (thisIMtx != mat3f::identity()) thisIMtx = inverse(thisIMtx);
  mat3f otherIMtx = mat3f(otherMtx);
  if (otherIMtx != mat3f::identity()) otherIMtx = inverse(otherIMtx);

  const ConvexShape** ptr = other->seperate();
  for (int i = 0; i < other->count(); i++) {
    if (ptr[i]->collides(this, otherMtx, otherIMtx, thisMtx, thisIMtx))
      return true;
  }
  return false;
}

float distanceToOrigin(vec3f a, vec3f b) {
  vec3f d = b - a;
  float t = dot(a, d) / length_squared(d);
  if (t < 0) return length(a);
  if (t > 1) return length(b);
  return length(a + t * d);
}
float distanceToOrigin(vec3f a, vec3f b, vec3f c) {
  vec3f v0 = b - a;
  vec3f v1 = c - a;
  vec3f n = cross(v0, v1);
  vec3f l = dot(a, n) * n;
  // |a b| |u| = |x|
  // |c d| |v| = |y|
  // |e f| |w| = |z|
  // u = (by-dx)/(bc-ad)
  // v = (ay-cx)/(ad-bc)
  float det = v0.x * v1.y - v0.y * v1.x;
  float u = -(v1.x * l.y - v1.y * l.x) / det;
  float v = (v0.x * l.y - v0.y * l.x) / det;
  if (u < 0) {
    if (v < 0)
      return length(a);
    else
      return distanceToOrigin(a, c);
  } else {
    if (v < 0)
      return distanceToOrigin(a, b);
    else {
      if (u + v < 1) {
        length(u * v0 + v * v1);
      } else {
        if (u > 1) {
          if (v < 1)
            return length(b);
          else
            return distanceToOrigin(b, c);
        }
        return length(c);
      }
    }
  }
}
float distanceToOrigin(Simplex pts) {
  switch (pts.size()) {
    case 1:
      return length(pts[0]);
    case 2:
      return distanceToOrigin(pts[0], pts[1]);
    case 3:
      return distanceToOrigin(pts[0], pts[1], pts[2]);
    case 4:
      float d = distanceToOrigin(pts[0], pts[1], pts[2]);
      d = std::min(d, distanceToOrigin(pts[0], pts[1], pts[3]));
      d = std::min(d, distanceToOrigin(pts[0], pts[2], pts[3]));
      d = std::min(d, distanceToOrigin(pts[1], pts[2], pts[3]));
      return d;
  }
}
float ConvexShape::distance(const ConvexShape* other, const mat4f& thisMtx,
                             const mat3f& thisIMtx, const mat4f& otherMtx,
                             const mat3f& otherIMtx) const {
  vec3f support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                       otherIMtx, {1, 0, 0});
  if (length_squared(support) == 0) return 0;
  Simplex pts;
  pts.push(support);

  size_t ite = 0;
  vec3f dir = -support;
  while (ite++ < MAX_ITERATIONS) {
    support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                   otherIMtx, dir);
    if (dot(support, dir) <= TOLERANCE) return distanceToOrigin(pts);
    pts.push(support);
    if (nextSimplex(pts, dir)) return 0;
  }
}
float ConvexShape::distance(const ConvexShape* other,
                             const mat4f& thisMtx = mat4f::identity(),
                             const mat4f& otherMtx = mat4f::identity()) const {
  mat3f thisIMtx = mat3f(thisMtx);
  if (thisIMtx != mat3f::identity()) thisIMtx = inverse(thisIMtx);
  mat3f otherIMtx = mat3f(otherMtx);
  if (otherIMtx != mat3f::identity()) otherIMtx = inverse(otherIMtx);

  return this->distance(other, thisMtx, thisIMtx, otherMtx, otherIMtx);
}

float ConvexShape::distance(const BoundingShape* other,
                             const mat4f& thisMtx = mat4f::identity(),
                             const mat4f& otherMtx = mat4f::identity()) const {
  mat3f thisIMtx = mat3f(thisMtx);
  if (thisIMtx != mat3f::identity()) thisIMtx = inverse(thisIMtx);
  mat3f otherIMtx = mat3f(otherMtx);
  if (otherIMtx != mat3f::identity()) otherIMtx = inverse(otherIMtx);

  float minDist = FLT_MAX;
  const ConvexShape** ptr = other->seperate();
  for (int i = 0; i < other->count(); i++) {
    minDist =
        std::min(ptr[i]->distance(this, otherMtx, otherIMtx, thisMtx, thisIMtx),
                 minDist);
  }
  return minDist;
}

size_t computeMinNormal(const std::vector<vec3f>& polytope,
                        const std::vector<vec3ull>& faces,
                        std::vector<vec4f>& normals) {
  size_t minI = 0;
  float minDist = std::numeric_limits<float>::max();

  int initSize = normals.size();
  for (int i = 0; i < initSize; i++)
    if (normals[i].w < minDist) {
      minDist = normals[i].w;
      minI = i;
    }

  for (size_t i = 0; i < faces.size(); i++) {
    vec3ull face = faces[i];
    vec3f norm = normalize(cross(polytope[face.y] - polytope[face.x],
                                 polytope[face.z] - polytope[face.x]));
    float dist = dot(polytope[face.x], norm);
    if (dist < 0) {
      norm = -norm;
      dist = -dist;
    }
    normals.push_back(vec4f(norm, dist));

    if (dist < minDist) {
      minDist = dist;
      minI = initSize + i;
    }
  }

  return minI;
}
size_t reconstruct(std::vector<vec3f>& polytope, std::vector<vec3ull>& faces,
                   std::vector<vec4f>& normals, vec3f support) {
  std::vector<std::pair<size_t, size_t>> edges;

  for (int i = 0; i < faces.size(); i++) {
    vec3ull face = faces[i];
    if (dot(vec3f(normals[i]), support - polytope[face.x]) >= 0) {
      for (int i = 0; i < 3; i++) {
        auto edge = std::find(edges.begin(), edges.end(),
                              std::make_pair(face[(i + 1) % 3], face[i]));

        if (edge != edges.end())
          edges.erase(edge);
        else
          edges.push_back(std::make_pair(face[i], face[(i + 1) % 3]));
      }

      faces[i] = faces.back();
      faces.pop_back();
      normals[i] = normals.back();
      normals.pop_back();
      i--;
    }
  }

  std::vector<vec3ull> newFaces;
  for (auto edge : edges) {
    newFaces.push_back(vec3ull(edge.first, edge.second, polytope.size()));
  }
  polytope.push_back(support);

  size_t ret = computeMinNormal(polytope, newFaces, normals);
  faces.insert(faces.end(), newFaces.begin(), newFaces.end());

  return ret;
}
vec4f ConvexShape::mtv(const ConvexShape* other, const mat4f& thisMtx,
                       const mat3f& thisIMtx, const mat4f& otherMtx,
                       const mat3f& otherIMtx) const {
  size_t ite = 0;
  // GJK
  Simplex pts;
  {
    vec3f support = ConvexShape::support(this, thisMtx, thisIMtx, other,
                                         otherMtx, otherIMtx, {1, 0, 0});
    pts.push(support);
    if (length_squared(support) != 0) {
      vec3f dir = -support;
      while (ite++ < MAX_ITERATIONS) {
        support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                       otherIMtx, dir);
        if (dot(support, dir) <= TOLERANCE) return vec4f(0, 0, 0, 0);
        pts.push(support);
        if (nextSimplex(pts, dir)) break;
      }
    }

    // Fix simplex
    if (pts.size() == 2) {
      vec3f l = normalize(pts[0] - pts[1]);
      vec3f p = vec3f(-l.y, l.x, l.z);
      pts.push(ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                    otherIMtx, p - dot(l, p) * l));
    }
    if (pts.size() == 3) {
      vec3f ba = pts[1] - pts[0];
      vec3f ca = pts[2] - pts[0];
      pts.push(ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                    otherIMtx, cross(ba, ca)));
    }
  }

  ite = 0;
  // EPA
  vec4f min;
  {
    std::vector<vec3f> polytope(pts.begin(), pts.end());
    std::vector<vec3ull> faces = {vec3ull(0, 1, 2), vec3ull(0, 3, 1),
                                  vec3ull(0, 2, 3), vec3ull(1, 3, 2)};
    std::vector<vec4f> normals;

    size_t minFace = computeMinNormal(polytope, faces, normals);
    while (ite++ < MAX_ITERATIONS) {
      min = normals[minFace];
      vec3f minNorm = vec3f(min);
      vec3f support = ConvexShape::support(this, thisMtx, thisIMtx, other,
                                           otherMtx, otherIMtx, minNorm);

      if (dot(support, minNorm) - min.w < 0.0001f) break;

      minFace = reconstruct(polytope, faces, normals, support);
    }
  }

  return vec4f(-min.x, -min.y, -min.z, min.w);
}
vec4f ConvexShape::mtv(const ConvexShape* other, const mat4f& thisMtx,
                       const mat4f& otherMtx) const {
  mat3f thisIMtx = mat3f(thisMtx);
  if (thisIMtx != mat3f::identity()) thisIMtx = inverse(thisIMtx);
  mat3f otherIMtx = mat3f(otherMtx);
  if (otherIMtx != mat3f::identity()) otherIMtx = inverse(otherIMtx);

  return this->mtv(other, thisMtx, thisIMtx, otherMtx, otherIMtx);
}

vec4f ConvexShape::mtv(const BoundingShape* other, const mat4f& thisMtx,
                       const mat4f& otherMtx) const {
  mat3f thisIMtx = mat3f(thisMtx);
  if (thisIMtx != mat3f::identity()) thisIMtx = inverse(thisIMtx);
  mat3f otherIMtx = mat3f(otherMtx);
  if (otherIMtx != mat3f::identity()) otherIMtx = inverse(otherIMtx);

  vec4f min = vec4f(0.0f, 0.0f, 0.0f, std::numeric_limits<float>::max());
  const ConvexShape** ptr = other->seperate();
  for (int i = 0; i < other->count(); i++) {
    vec4f v = ptr[i]->mtv(this, otherMtx, otherIMtx, thisMtx, thisIMtx);
    if (v.w > 0 && min.w > v.w) {
      min = v;
    }
  }
  return vec4f(-min.x, -min.y, -min.z, min.w);
}



bool ConvexShape::contains(const vec3f& point, const mat4f& thisMtx,
                           const mat4f& otherMtx) const {
  return this->collides(new PointShape(point), thisMtx, otherMtx);
}
float ConvexShape::intersects(const Ray& ray, const mat4f& thisMtx) const {
  mat3f thisIMtx = mat3f(thisMtx);
  if (thisIMtx != mat3f::identity()) thisIMtx = inverse(thisIMtx);

  vec3f mn =
      proj(vec3f(thisMtx * this->furthestPoint(-thisIMtx * ray.dir)), ray.dir);
  vec3f mx =
      proj(vec3f(thisMtx * this->furthestPoint(thisIMtx * ray.dir)), ray.dir);
  if (dot(mx - ray.src, ray.dir) / length_squared(ray.dir) < 0) return -1;
  LineShape* line = new LineShape(max(ray.src, mn), max(ray.src, mx));

  if (!this->collides(line, thisMtx)) return -1;

  int n = 0;
  while (n++ < MAX_ITERATIONS && distance_squared(line->a, line->b) > 0.0001f) {
    if (this->collides(line, thisMtx)) {
      // std::cout << "Collides: " << line->a << " " << line->b << std::endl;
      line->b = (line->b + line->a) / 2.0f;
    } else {
      // std::cout << "Misses: " << line->a << " " << line->b << std::endl;
      line->b = (line->b - line->a) + line->b;
      line->a = (line->a + line->b) / 2.0f;
    }
  }
  float t = dot((line->b + line->a) / 2.0f - ray.src, ray.dir) /
            length_squared(ray.dir);
  delete line;
  return t;
}
