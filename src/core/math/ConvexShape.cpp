#pragma once

#include "core/math/shape/ConvexShape.h"

#include <algorithm>
#include <limits>
#include <vector>

#include "core/math/shape/LineShape.h"
#include "core/math/shape/PointShape.h"
#include "core/math/shape/Simplex.h"

std::ostream& operator<<(std::ostream& os, Simplex& simplex) {
  os << "{";
  for (int i = 0; i < simplex.size(); i++) os << simplex[i] << ", ";
  os << "}";
  return os;
}

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
    // if (length_squared(dir) == 0) return true;
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
    } else {
      return line(pts = {a, b}, dir);
    }
  } else {
    if (sameDir(cross(ab, abc), ao)) {
      return line(pts = {a, b}, dir);
    } else {
      if (sameDir(abc, ao)) {
        dir = abc;
      } else {
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
  Simplex pts;
  pts.push(support);

  size_t ite = 0;
  vec3f dir = -support;
  while (ite++ < MAX_ITERATIONS) {
    if (length_squared(dir) == 0) return true;
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

vec3f vecToOrigin(vec3f a, vec3f b) {
  vec3f d = b - a;
  float t = -dot(a, d) / length_squared(d);
  if (t < 0) a;
  if (t > 1) b;
  return a + t * d;
}
vec3f vecToOrigin(vec3f a, vec3f b, vec3f c) {
  vec3f v0 = b - a;
  vec3f v1 = c - a;
  vec3f n = normalize(cross(v0, v1));
  vec3f l = dot(a, n) * n;
  mat3f M = mat3f(a, v0, v1);
  vec3f s = inverse(M) * l;
  float u = s.y / s.x, v = s.z / s.x;
  if (u < 0) {
    if (v < 0)
      return a;
    else
      return vecToOrigin(a, c);
  } else {
    if (v < 0) {
      return vecToOrigin(a, b);
    } else {
      if (u + v < 1) {
        return l;
      } else {
        if (u > 1) {
          if (v < 1)
            return b;
          else
            return vecToOrigin(b, c);
        }
        return c;
      }
    }
  }
}
vec3f vecToOrigin(Simplex pts) {
  switch (pts.size()) {
    case 1:
      return pts[0];
    case 2:
      return vecToOrigin(pts[0], pts[1]);
    case 3:
      return vecToOrigin(pts[0], pts[1], pts[2]);
    case 4:
      vec3f m = vecToOrigin(pts[0], pts[1], pts[2]);
      vec3f n = vecToOrigin(pts[0], pts[2], pts[3]);
      if (length_squared(n) < length_squared(m)) m = n;
      n = vecToOrigin(pts[0], pts[1], pts[3]);
      if (length_squared(n) < length_squared(m)) m = n;
      n = vecToOrigin(pts[1], pts[2], pts[3]);
      if (length_squared(n) < length_squared(m)) m = n;
      return m;
  }
}
bool degenrate(Simplex pts, vec3f support) {
  switch (pts.size()) {
    case 1:
      return distance_squared(pts[0], support) <= TOLERANCE * TOLERANCE;
    case 2:
      return length_squared(cross(pts[1] - pts[0], support - pts[0])) <=
             TOLERANCE * TOLERANCE;
    case 3:
      return dot(cross(pts[1] - pts[0], pts[2] - pts[0]), support - pts[0]) <=
             TOLERANCE * TOLERANCE;
    case 4:
      return true;
  }
}
vec3f originPointingNormal(Simplex pts) {
  // std::cout << "n:" << pts.size() << std::endl;
  vec3f norm;
  switch (pts.size()) {
    case 1:
      return -pts[0];
    case 2:
      return normalize(cross(cross(pts[1] - pts[0], -pts[0]), pts[1] - pts[0]));
    case 3:
      norm = normalize(cross(pts[1] - pts[0], pts[2] - pts[0]));
      if (dot(norm, pts[0]) < 0) return norm;
      return -norm;
    case 4:
      vec3f avg = (pts[0] + pts[1] + pts[2] + pts[3]) / 4.0f;
      vec3f avg_n = normalize(avg);
      norm = normalize(cross(pts[1] - pts[0], pts[2] - pts[0]));
      if (dot(norm, pts[0] - avg) < 0) norm = -norm;  // Outward facing normal
      float d = dot(avg_n, norm);

      vec3f normt = normalize(cross(pts[2] - pts[0], pts[3] - pts[0]));
      if (dot(norm, pts[0] - avg) < 0) norm = -norm;  // Outward facing normal
      float dt = dot(avg_n, norm);
      if (dt < d) {
        d = dt;
        norm = normt;
      }

      normt = normalize(cross(pts[1] - pts[0], pts[3] - pts[0]));
      if (dot(norm, pts[0] - avg) < 0) norm = -norm;  // Outward facing normal
      dt = dot(avg_n, norm);
      if (dt < d) {
        d = dt;
        norm = normt;
      }

      normt = normalize(cross(pts[2] - pts[1], pts[3] - pts[1]));
      if (dot(norm, pts[1] - avg) < 0) norm = -norm;  // Outward facing normal
      dt = dot(avg_n, norm);
      if (dt < d) {
        d = dt;
        norm = normt;
      }

      return norm;
  }
}
vec3f ConvexShape::distance(const ConvexShape* other, const mat4f& thisMtx,
                            const mat3f& thisIMtx, const mat4f& otherMtx,
                            const mat3f& otherIMtx) const {
  vec3f support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                       otherIMtx, {1, 0, 0});
  Simplex pts;
  pts.push(support);

  size_t ite = 0;
  vec3f dir = length_squared(support) == 0 ? vec3f(0, 1, 0) : -support;
  vec3f dist = vecToOrigin(pts);
  vec3f prevDist = dist;
  while (ite++ < MAX_ITERATIONS) {
    support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                   otherIMtx, dir);
    if (degenrate(pts, support)) return dist;
    pts.push(support);
    // std::cout << pts << std::endl;
    prevDist = dist;
    dist = vecToOrigin(pts);
    if (length_squared(prevDist) - length_squared(dist) <=
        TOLERANCE * TOLERANCE)
      return prevDist;
    if (nextSimplex(pts, dir)) return vec3f(0, 0, 0);
    // std::cout << pts << std::endl << std::endl;
    dir = normalize(dir);
  }
  return vecToOrigin(pts);
}
vec3f ConvexShape::distance(const ConvexShape* other, const mat4f& thisMtx,
                            const mat4f& otherMtx) const {
  mat3f thisIMtx = mat3f(thisMtx);
  if (thisIMtx != mat3f::identity()) thisIMtx = inverse(thisIMtx);
  mat3f otherIMtx = mat3f(otherMtx);
  if (otherIMtx != mat3f::identity()) otherIMtx = inverse(otherIMtx);

  return this->distance(other, thisMtx, thisIMtx, otherMtx, otherIMtx);
}

vec3f ConvexShape::distance(const BoundingShape* other, const mat4f& thisMtx,
                            const mat4f& otherMtx) const {
  mat3f thisIMtx = mat3f(thisMtx);
  if (thisIMtx != mat3f::identity()) thisIMtx = inverse(thisIMtx);
  mat3f otherIMtx = mat3f(otherMtx);
  if (otherIMtx != mat3f::identity()) otherIMtx = inverse(otherIMtx);

  const ConvexShape** ptr = other->seperate();
  vec3f minVec = ptr[0]->distance(this, otherMtx, otherIMtx, thisMtx, thisIMtx);
  float minDist = length_squared(minVec);
  for (int i = 1; i < other->count(); i++) {
    vec3f vec = ptr[i]->distance(this, otherMtx, otherIMtx, thisMtx, thisIMtx);
    float dist = length_squared(vec);
    if (dist < minDist) {
      minDist = dist;
      minVec = vec;
    }
  }
  return minVec;
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
        if (length_squared(dir) == 0) break;
        support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                       otherIMtx, dir);
        if (dot(support, dir) <= TOLERANCE) return vec4f(0, 0, 0, 0);
        pts.push(support);
        if (nextSimplex(pts, dir)) break;
      }
    }

    // Fix simplex
    if (pts.size() == 1) {
      pts.push(ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx,
                                    otherIMtx, vec3f(0, 1, 0)));
    }
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
