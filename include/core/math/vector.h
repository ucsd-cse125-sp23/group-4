#pragma once

#include <iostream>
#include <algorithm>

template <typename T>
struct TVector4;

template <typename T>
struct TVector3 {
    TVector3() {}
    template <typename T2>
    TVector3(T2 x, T2 y, T2 z) : x(T(x)), y(T(y)), z(T(z)) {}
    template <typename T2>
    TVector3(const TVector3<T2>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)) {}
    template <typename T2>
    TVector3(const T2* v) : x(T(v[0])), y(T(v[1])), z(T(v[2])) {}
    template <typename T2>
    TVector3(const TVector4<T2>& v) : x(T(v[0])), y(T(v[1])), z(T(v[2])) {}

    T& operator[](int i) {
        return *(&x + i);
    }

    T operator[](int i) const {
        return *(&x + i);
    }

    T x, y, z;
};
template <typename T>
inline bool operator==(const TVector3<T>& v0, const TVector3<T>& v1) { return v0.x == v1.x && v0.y == v1.y && v0.z == v1.z; }
template <typename T>
inline bool operator!=(const TVector3<T>& v0, const TVector3<T>& v1) { return !(v0 == v1); }
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const TVector3<T>& v) {
    return os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
}

template <typename T>
struct TVector4 {
    TVector4() {}
    template <typename T2>
    TVector4(T2 x, T2 y, T2 z, T2 w) : x(T(x)), y(T(y)), z(T(z)), w(T(w)) {}
    template <typename T2>
    TVector4(const TVector4<T2>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {}
    template <typename T2>
    TVector4(const T2* v) : x(T(v[0])), y(T(v[1])), z(T(v[2])), w(T(v[3])) {}
    template <typename T2>
    TVector4(const TVector3<T2>& v, T2 w) : x(T(v[0])), y(T(v[1])), z(T(v[2])), w(w) {}


    T& operator[](int i) {
        return *(&x + i);
    }

    T operator[](int i) const {
        return *(&x + i);
    }

    T x, y, z, w;
};
template <typename T>
inline bool operator==(const TVector4<T>& v0, const TVector4<T>& v1) { return v0.x == v1.x && v0.y == v1.y && v0.z == v1.z && v0.w == v1.w; }
template <typename T>
inline bool operator!=(const TVector4<T>& v0, const TVector4<T>& v1) { return !(v0 == v1); }
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const TVector4<T>& v) {
    return os << "(" << v[0] << ", " << v[1] << ", " << v[2] <<", " << v[3] << ")";
}

using vec3ull = TVector3<size_t>;

using vec3i = TVector3<int>;
using vec3f = TVector3<float>;
using vec3d = TVector3<double>;
using vec4i = TVector4<int>;
using vec4f = TVector4<float>;
using vec4d = TVector4<double>;

template <typename T>
inline TVector3<T> operator+(const TVector3<T>& v0, const TVector3<T>& v1) {
    return TVector3<T>(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z);
}
template <typename T>
inline TVector3<T> operator+(const TVector3<T>& v, const T& s) {
    return TVector3<T>(v.x + s, v.y + s, v.z + s);
}
template <typename T>
inline TVector3<T> operator+(const T& s, const TVector3<T>& v) {
    return TVector3<T>(s + v.x, s + v.y, s + v.z);
}
template <typename T>
inline TVector3<T>& operator+=(TVector3<T>& v0, const TVector3<T>& v1) {
    v0.x += v1.x;
    v0.y += v1.y;
    v0.z += v1.z;
    return v0;
}
template <typename T>
inline TVector3<T>& operator+=(TVector3<T>& v, const T& s) {
    v.x += s;
    v.y += s;
    v.z += s;
    return v;
}
template <typename T>
inline TVector3<T> operator-(const TVector3<T>& v0, const TVector3<T>& v1) {
    return TVector3<T>(v0.x - v1.x, v0.y - v1.y, v0.z - v1.z);
}
template <typename T>
inline TVector3<T> operator-(const T& s, const TVector3<T>& v) {
    return TVector3<T>(s - v.x, s - v.y, s - v.z);
}
template <typename T>
inline TVector3<T> operator-(const TVector3<T>& v, const T& s) {
    return TVector3<T>(v.x - s, v.y - s, v.z - s);
}
template <typename T>
inline TVector3<T>& operator-=(TVector3<T>& v0, const TVector3<T>& v1) {
    v0.x -= v1.x;
    v0.y -= v1.y;
    v0.z -= v1.z;
    return v0;
}
template <typename T>
inline TVector3<T>& operator-=(TVector3<T>& v, const T& s) {
    v.x -= s;
    v.y -= s;
    v.z -= s;
    return v;
}
template <typename T>
inline TVector3<T> operator-(const TVector3<T>& v) {
    return TVector3<T>(-v.x, -v.y, -v.z);
}
template <typename T>
inline TVector3<T> operator*(const T& s, const TVector3<T>& v) {
    return TVector3<T>(s * v[0], s * v[1], s * v[2]);
}
template <typename T>
inline TVector3<T> operator*(const TVector3<T>& v, const T& s) {
    return TVector3<T>(v[0] * s, v[1] * s, v[2] * s);
}
template <typename T>
inline TVector3<T> operator*(const TVector3<T>& v0, const TVector3<T>& v1) {
    return TVector3<T>(v0[0] * v1[0], v0[1] * v1[1], v0[2] * v1[2]);
}
template <typename T>
inline TVector3<T>& operator*=(TVector3<T>& v, const T& s) {
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
    return v;
}
template <typename T>
inline TVector3<T>& operator*=(TVector3<T>& v0, const TVector3<T>& v1) {
    v0[0] *= v1[0];
    v0[1] *= v1[1];
    v0[2] *= v1[2];
    return v0;
}
template <typename T>
inline TVector3<T> operator/(const TVector3<T>& v, const T& s) {
    T inv_s = T(1) / s;
    return TVector3<T>(v[0] * inv_s, v[1] * inv_s, v[2] * inv_s);
}
template <typename T>
inline TVector3<T> operator/(const T& s, const TVector3<T>& v) {
    return TVector3<T>(s / v[0], s / v[1], s / v[2]);
}
template <typename T>
inline TVector3<T> operator/(const TVector3<T>& v0, const TVector3<T>& v1) {
    return TVector3<T>(v0[0] / v1[0], v0[1] / v1[1], v0[2] / v1[2]);
}
template <typename T>
inline TVector3<T>& operator/=(TVector3<T>& v, const T& s) {
    T inv_s = T(1) / s;
    v *= inv_s;
    return v;
}
template <typename T>
inline T dot(const TVector3<T>& v0, const TVector3<T>& v1) {
    return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}
template <typename T>
inline TVector3<T> cross(const TVector3<T>& v0, const TVector3<T>& v1) {
    return TVector3<T>{
        v0[1] * v1[2] - v0[2] * v1[1],
        v0[2] * v1[0] - v0[0] * v1[2],
        v0[0] * v1[1] - v0[1] * v1[0]};
}
template <typename T>
inline T distance_squared(const TVector3<T>& v0, const TVector3<T>& v1) {
    return dot(v0 - v1, v0 - v1);
}
template <typename T>
inline T distance(const TVector3<T>& v0, const TVector3<T>& v1) {
    return sqrt(distance_squared(v0, v1));
}
template <typename T>
inline T length_squared(const TVector3<T>& v) {
    return dot(v, v);
}
template <typename T>
inline T length(const TVector3<T>& v) {
    return sqrt(length_squared(v));
}
template <typename T>
inline TVector3<T> normalize(const TVector3<T>& v0) {
    auto l = length_squared(v0);
    if (l <= 0) {
        return TVector3<T>{0, 0, 0};
    }
    else {
        return v0 / sqrt(l);
    }
}
template <typename T>
inline T average(const TVector3<T>& v) {
    return (v.x + v.y + v.z) / 3;
}
template <typename T>
inline T max(const TVector3<T>& v) {
    return std::max(std::max(v.x, v.y), v.z);
}
template <typename T>
inline T min(const TVector3<T>& v) {
    return std::min(std::min(v.x, v.y), v.z);
}
template <typename T>
inline TVector3<T> min(const TVector3<T>& v0, const TVector3<T>& v1) {
    return TVector3<T>{std::min(v0.x, v1.x), std::min(v0.y, v1.y), std::min(v0.z, v1.z)};
}
template <typename T>
inline TVector3<T> max(const TVector3<T>& v0, const TVector3<T>& v1) {
    return TVector3<T>{std::max(v0.x, v1.x), std::max(v0.y, v1.y), std::max(v0.z, v1.z)};
}


template <typename T>
inline TVector4<T> operator+(const TVector4<T>& v0, const TVector4<T>& v1) {
    return TVector4<T>(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z, v0.w + v1.w);
}
template <typename T>
inline TVector4<T> operator+(const TVector4<T>& v, const T& s) {
    return TVector4<T>(v.x + s, v.y + s, v.z + s, v.w + s);
}
template <typename T>
inline TVector4<T> operator+(const T& s, const TVector4<T>& v) {
    return TVector4<T>(s + v.x, s + v.y, s + v.z, s + v.w);
}
template <typename T>
inline TVector4<T>& operator+=(TVector4<T>& v0, const TVector4<T>& v1) {
    v0.x += v1.x;
    v0.y += v1.y;
    v0.z += v1.z;
    v0.w += v1.w;
    return v0;
}
template <typename T>
inline TVector4<T>& operator+=(TVector4<T>& v, const T& s) {
    v.x += s;
    v.y += s;
    v.z += s;
    v.w += s;
    return v;
}
template <typename T>
inline TVector4<T> operator-(const TVector4<T>& v0, const TVector4<T>& v1) {
    return TVector4<T>(v0.x - v1.x, v0.y - v1.y, v0.z - v1.z, v0.w - v1.w);
}
template <typename T>
inline TVector4<T> operator-(const T& s, const TVector4<T>& v) {
    return TVector4<T>(s - v.x, s - v.y, s - v.z, s - v.w);
}
template <typename T>
inline TVector4<T> operator-(const TVector4<T>& v, const T& s) {
    return TVector4<T>(v.x - s, v.y - s, v.z - s, v.w - s);
}
template <typename T>
inline TVector4<T>& operator-=(TVector4<T>& v0, const TVector4<T>& v1) {
    v0.x -= v1.x;
    v0.y -= v1.y;
    v0.z -= v1.z;
    v0.w -= v1.w;
    return v0;
}
template <typename T>
inline TVector4<T>& operator-=(TVector4<T>& v, const T& s) {
    v.x -= s;
    v.y -= s;
    v.z -= s;
    v.w -= s;
    return v;
}
template <typename T>
inline TVector4<T> operator-(const TVector4<T>& v) {
    return TVector4<T>(-v.x, -v.y, -v.z, -v.w);
}
template <typename T>
inline TVector4<T> operator*(const T& s, const TVector4<T>& v) {
    return TVector4<T>(s * v[0], s * v[1], s * v[2], s * v[3]);
}
template <typename T>
inline TVector4<T> operator*(const TVector4<T>& v, const T& s) {
    return TVector4<T>(v[0] * s, v[1] * s, v[2] * s, v[3] * s);
}
template <typename T>
inline TVector4<T> operator*(const TVector4<T>& v0, const TVector4<T>& v1) {
    return TVector4<T>(v0[0] * v1[0], v0[1] * v1[1], v0[2] * v1[2], v0[3] * v1[3]);
}
template <typename T>
inline TVector4<T>& operator*=(TVector4<T>& v, const T& s) {
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
    v[3] *= s;
    return v;
}
template <typename T>
inline TVector4<T>& operator*=(TVector4<T>& v0, const TVector4<T>& v1) {
    v0[0] *= v1[0];
    v0[1] *= v1[1];
    v0[2] *= v1[2];
    v0[3] *= v1[3];
    return v0;
}
template <typename T>
inline TVector4<T> operator/(const TVector4<T>& v, const T& s) {
    T inv_s = T(1) / s;
    return TVector4<T>(v[0] * inv_s, v[1] * inv_s, v[2] * inv_s, v[3] * inv_s);
}
template <typename T>
inline TVector4<T> operator/(const T& s, const TVector4<T>& v) {
    return TVector4<T>(s / v[0], s / v[1], s / v[2], s / v[3]);
}
template <typename T>
inline TVector4<T> operator/(const TVector4<T>& v0, const TVector4<T>& v1) {
    return TVector4<T>(v0[0] / v1[0], v0[1] / v1[1], v0[2] / v1[2], v0[3] / v1[3]);
}
template <typename T>
inline TVector4<T>& operator/=(TVector4<T>& v, const T& s) {
    T inv_s = T(1) / s;
    v *= inv_s;
    return v;
}
template <typename T>
inline T dot(const TVector4<T>& v0, const TVector4<T>& v1) {
    return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2] + v0[3] * v1[3];
}
template <typename T>
inline T distance_squared(const TVector4<T>& v0, const TVector4<T>& v1) {
    return dot(v0 - v1, v0 - v1);
}
template <typename T>
inline T distance(const TVector4<T>& v0, const TVector4<T>& v1) {
    return sqrt(distance_squared(v0, v1));
}
template <typename T>
inline T length_squared(const TVector4<T>& v) {
    return dot(v, v);
}
template <typename T>
inline T length(const TVector4<T>& v) {
    return sqrt(length_squared(v));
}
template <typename T>
inline TVector4<T> normalize(const TVector4<T>& v0) {
    auto l = length_squared(v0);
    if (l <= 0) {
        return TVector4<T>{0, 0, 0, 0};
    }
    else {
        return v0 / sqrt(l);
    }
}
template <typename T>
inline T average(const TVector4<T>& v) {
    return (v.x + v.y + v.z + v.w) / 4;
}
template <typename T>
inline T max(const TVector4<T>& v) {
    return std::max(std::max(v.x, v.y), std::max(v.z, v.w));
}
template <typename T>
inline T min(const TVector4<T>& v) {
    return std::min(std::min(v.x, v.y), std::min(v.z, v.w));
}
template <typename T>
inline TVector4<T> min(const TVector4<T>& v0, const TVector4<T>& v1) {
    return TVector4<T>{std::min(v0.x, v1.x), std::min(v0.y, v1.y), std::min(v0.z, v1.z), std::min(v0.w, v1.w)};
}
template <typename T>
inline TVector4<T> max(const TVector4<T>& v0, const TVector4<T>& v1) {
    return TVector4<T>{std::max(v0.x, v1.x), std::max(v0.y, v1.y), std::max(v0.z, v1.z), std::max(v0.w, v1.w)};
}


template <typename T>
inline TVector3<T> tangent(const TVector3<T>& vec, const TVector3<T>& norm) {
    return vec - dot(vec, norm) * vec;
}


template <typename T>
inline TVector3<T> sameSign(const TVector3<T>& v0, const TVector3<T>& v1) {
    return TVector3<T>((v0.x > 0) ^ (v1.x <= 0), (v0.y > 0) ^ (v1.y <= 0), (v0.z > 0) ^ (v1.z <= 0));
}
/**
 * Clamp vec to between 0 and sign of ref
 * ex. ref = (1,-1,1) and vec = (-1,-2,0) => returns (0,-2,0)
 */
template <typename T>
inline TVector3<T> clampBySign(const TVector3<T>& ref, const TVector3<T>& vec) {
    return vec * sameSign(ref, vec);
}