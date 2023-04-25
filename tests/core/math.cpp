#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "core/math/vector.h"
#include "core/math/matrix.h"

using Catch::Matchers::WithinAbs;

TEST_CASE("Basic Vector Operations", "[vector]") {
	vec3i u = vec3i(1, 2, 3);
	vec3i v = vec3i(2, 3, 4);
	vec3f a = vec3f(u);
	vec3f b = vec3f(v);
	
	CHECK(u + v == vec3i(3, 5, 7));
	CHECK(u - v == vec3i(-1, -1, -1));
	CHECK(u * v == vec3i(2, 6, 12));
	CHECK_THAT((a / b).x, WithinAbs(0.500, 0.001));
	CHECK_THAT((a / b).y, WithinAbs(0.666, 0.001));
	CHECK_THAT((a / b).z, WithinAbs(0.750, 0.001));
	CHECK(u + 2 == vec3i(3, 4, 5));
	CHECK(u - 2 == vec3i(-1, 0, 1));
	CHECK(u * 2 == vec3i(2, 4, 6));
	CHECK_THAT((a / 2.0f).x, WithinAbs(0.500, 0.001));
	CHECK_THAT((a / 2.0f).y, WithinAbs(1.000, 0.001));
	CHECK_THAT((a / 2.0f).z, WithinAbs(1.500, 0.001));
	CHECK(2 + v == vec3i(4, 5, 6));
	CHECK(2 - v == vec3i(0, -1, -2));
	CHECK(2 * v == vec3i(4, 6, 8));
	CHECK_THAT((2.0f / a).x, WithinAbs(2.000, 0.001));
	CHECK_THAT((2.0f / a).y, WithinAbs(1.000, 0.001));
	CHECK_THAT((2.0f / a).z, WithinAbs(0.666, 0.001));

	CHECK(dot(u,v) == 20);
	CHECK(cross(vec3i(1, 0, 0), vec3i(0, 1, 0)) == vec3i(0, 0, 1));

	CHECK_THAT(distance_squared(a, b), WithinAbs(3.000, 0.001));
	CHECK_THAT(distance(a, b), WithinAbs(1.732, 0.001));
	CHECK_THAT(length_squared(a), WithinAbs(14.000, 0.001));
	CHECK_THAT(length(a), WithinAbs(3.741, 0.001));

	CHECK_THAT(normalize(a).x, WithinAbs(0.267, 0.001));
	CHECK_THAT(normalize(a).y, WithinAbs(0.534, 0.001));
	CHECK_THAT(normalize(a).z, WithinAbs(0.801, 0.001));
	CHECK_THAT(average(a), WithinAbs(2.000, 0.001));
	CHECK_THAT(min(a), WithinAbs(1.000, 0.001));
	CHECK_THAT(max(a), WithinAbs(3.000, 0.001));
	CHECK(min(a, b) == a);
	CHECK(max(a, b) == b);


	vec4i w = vec4i(1, 2, 3, 4);
	vec4i x = vec4i(2, 3, 4, 5);
	vec4f c = vec4f(w);
	vec4f d = vec4f(x);

	CHECK(w + x == vec4i(3, 5, 7, 9));
	CHECK(w - x == vec4i(-1, -1, -1, -1));
	CHECK(w * x == vec4i(2, 6, 12, 20));
	CHECK_THAT((c / d).x, WithinAbs(0.500, 0.001));
	CHECK_THAT((c / d).y, WithinAbs(0.666, 0.001));
	CHECK_THAT((c / d).z, WithinAbs(0.750, 0.001));
	CHECK_THAT((c / d).w, WithinAbs(0.800, 0.001));
	CHECK(w + 2 == vec4i(3, 4, 5, 6));
	CHECK(w - 2 == vec4i(-1, 0, 1, 2));
	CHECK(w * 2 == vec4i(2, 4, 6, 8));
	CHECK_THAT((c / 2.0f).x, WithinAbs(0.500, 0.001));
	CHECK_THAT((c / 2.0f).y, WithinAbs(1.000, 0.001));
	CHECK_THAT((c / 2.0f).z, WithinAbs(1.500, 0.001));
	CHECK_THAT((c / 2.0f).w, WithinAbs(2.000, 0.001));
	CHECK(2 + x == vec4i(4, 5, 6, 7));
	CHECK(2 - x == vec4i(0, -1, -2, -3));
	CHECK(2 * x == vec4i(4, 6, 8, 10));
	CHECK_THAT((2.0f / c).x, WithinAbs(2.000, 0.001));
	CHECK_THAT((2.0f / c).y, WithinAbs(1.000, 0.001));
	CHECK_THAT((2.0f / c).z, WithinAbs(0.666, 0.001));
	CHECK_THAT((2.0f / c).w, WithinAbs(0.500, 0.001));

	CHECK(dot(w, x) == 40);

	CHECK_THAT(distance_squared(c, d), WithinAbs(4.000, 0.001));
	CHECK_THAT(distance(c, d), WithinAbs(2.000, 0.001));
	CHECK_THAT(length_squared(c), WithinAbs(30.000, 0.001));
	CHECK_THAT(length(c), WithinAbs(5.477, 0.001));

	CHECK_THAT(normalize(c).x, WithinAbs(0.183, 0.001));
	CHECK_THAT(normalize(c).y, WithinAbs(0.365, 0.001));
	CHECK_THAT(normalize(c).z, WithinAbs(0.548, 0.001));
	CHECK_THAT(normalize(c).w, WithinAbs(0.730, 0.001));
	CHECK_THAT(average(c), WithinAbs(2.500, 0.001));
	CHECK_THAT(min(c), WithinAbs(1.000, 0.001));
	CHECK_THAT(max(c), WithinAbs(4.000, 0.001));
	CHECK(min(c, d) == c);
	CHECK(max(c, d) == d);
}
TEST_CASE("Assignment Vector Operations", "[vector]") {
	vec3f u = vec3f(1, 2, 3);
	vec3f v = vec3f(1, 1, 1);

	u += v;
	CHECK(u == vec3f(2, 3, 4));
	u -= v;
	CHECK(u == vec3f(1, 2, 3));
	u *= u;
	CHECK(u == vec3f(1, 4, 9));

	u += 2.0f;
	CHECK(u == vec3f(3, 6, 11));
	u -= 2.0f;
	CHECK(u == vec3f(1, 4, 9));
	u *= 2.0f;
	CHECK(u == vec3f(2, 8, 18));
}
TEST_CASE("Special Vector Operations", "[vector]") {
	vec3f v = vec3f(1, 1, 0);
}

TEST_CASE("Baisc Matrix Operations", "[matrix]") {
	mat3f m0(1, 2, 3, 4, 5, 6, 7, 8, 9);

	CHECK(m0 + 2.0f == mat3f(3, 4, 5, 6, 7, 8, 9, 10, 11));
	CHECK(m0 - 2.0f == mat3f(-1, 0, 1, 2, 3, 4, 5, 6, 7));
	CHECK(m0 * 2.0f == mat3f(2, 4, 6, 8, 10, 12, 14, 16, 18));
	CHECK(2.0f + m0 == mat3f(3, 4, 5, 6, 7, 8, 9, 10, 11));
	CHECK(-(2.0f - m0) == mat3f(-1, 0, 1, 2, 3, 4, 5, 6, 7));
	CHECK(2.0f * m0 == mat3f(2, 4, 6, 8, 10, 12, 14, 16, 18));
	CHECK(m0 + m0 == mat3f(2, 4, 6, 8, 10, 12, 14, 16, 18));
	CHECK(m0 - m0 == mat3f(0, 0, 0, 0, 0, 0, 0, 0, 0));
	CHECK(eMul(m0, m0) == mat3f(1, 4, 9, 16, 25, 36, 49, 64, 81));
	CHECK(eDiv(m0, m0) == mat3f(1, 1, 1, 1, 1, 1, 1, 1, 1));
	CHECK(m0 * m0 == mat3f(30, 36, 42, 66, 81, 96, 102, 126, 150));
	CHECK(transpose(m0) == mat3f(1, 4, 7, 2, 5, 8, 3, 6, 9));
	m0 = mat3f(1, 1, 3, 2, 3, 4, 5, 1, 2);
	m0 = inverse(m0) * m0;
	CHECK_THAT(m0(0, 0), WithinAbs(1, 0.001));
	CHECK_THAT(m0(0, 1), WithinAbs(0, 0.001));
	CHECK_THAT(m0(0, 2), WithinAbs(0, 0.001));
	CHECK_THAT(m0(1, 0), WithinAbs(0, 0.001));
	CHECK_THAT(m0(1, 1), WithinAbs(1, 0.001));
	CHECK_THAT(m0(1, 2), WithinAbs(0, 0.001));
	CHECK_THAT(m0(2, 0), WithinAbs(0, 0.001));
	CHECK_THAT(m0(2, 1), WithinAbs(0, 0.001));
	CHECK_THAT(m0(2, 2), WithinAbs(1, 0.001));
	m0 = mat3f(1, 2, 3, 4, 5, 6, 7, 8, 9);
	CHECK(m0 * vec3f(1, 2, 3) == vec3f(14, 32, 50));


	mat4f m1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

	CHECK(m1 + 2.0f == mat4f(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18));
	CHECK(m1 - 2.0f == mat4f(-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14));
	CHECK(m1 * 2.0f == mat4f(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32));
	CHECK(2.0f + m1 == mat4f(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18));
	CHECK(-(2.0f - m1) == mat4f(-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14));
	CHECK(2.0f * m1 == mat4f(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32));
	CHECK(m1 + m1 == mat4f(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32));
	CHECK(m1 - m1 == mat4f(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
	CHECK(eMul(m1, m1) == mat4f(1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225, 256));
	CHECK(eDiv(m1, m1) == mat4f(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));
	CHECK(m1 * m1 == mat4f(90, 100, 110, 120, 202, 228, 254, 280, 314, 356, 398, 440, 426, 484, 542, 600));
	CHECK(transpose(m1) == mat4f(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16));
	m1 = mat4f(1, 7, 5, 3, 3, 4, 2, 1, 5, 7, 7, 9, 7, 4, 2, 8);
	m1 = inverse(m1) * m1;
	CHECK_THAT(m1(0, 0), WithinAbs(1, 0.001));
	CHECK_THAT(m1(0, 1), WithinAbs(0, 0.001));
	CHECK_THAT(m1(0, 2), WithinAbs(0, 0.001));
	CHECK_THAT(m1(0, 3), WithinAbs(0, 0.001));
	CHECK_THAT(m1(1, 0), WithinAbs(0, 0.001));
	CHECK_THAT(m1(1, 1), WithinAbs(1, 0.001));
	CHECK_THAT(m1(1, 2), WithinAbs(0, 0.001));
	CHECK_THAT(m1(1, 3), WithinAbs(0, 0.001));
	CHECK_THAT(m1(2, 0), WithinAbs(0, 0.001));
	CHECK_THAT(m1(2, 1), WithinAbs(0, 0.001));
	CHECK_THAT(m1(2, 2), WithinAbs(1, 0.001));
	CHECK_THAT(m1(2, 3), WithinAbs(0, 0.001));
	CHECK_THAT(m1(3, 0), WithinAbs(0, 0.001));
	CHECK_THAT(m1(3, 1), WithinAbs(0, 0.001));
	CHECK_THAT(m1(3, 2), WithinAbs(0, 0.001));
	CHECK_THAT(m1(3, 3), WithinAbs(1, 0.001));
	m1 = mat4f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CHECK(m1 * vec4f(1, 2, 3, 4) == vec4f(30, 70, 110, 150));
}
TEST_CASE("Transformation Matrices", "[matrix]") {
	mat4f T = translate(vec3f(2, -1, 3));
	CHECK(T * vec4f(1, 2, 3, 1) == vec4f(3, 1, 6, 1));
	CHECK(T * vec4f(5, 5, 5, 1) == vec4f(7, 4, 8, 1));
	CHECK(T * vec4f(1, 2, 3, 0) == vec4f(1, 2, 3, 0));
	mat4f S = scale(vec3f(2, 3, 4));
	CHECK(S * vec4f(1, 2, 3, 1) == vec4f(2, 6, 12, 1));
	CHECK(S * vec4f(5, 5, 5, 1) == vec4f(10, 15, 20, 1));
	CHECK(S * vec4f(1, 2, 3, 0) == vec4f(2, 6, 12, 0));

	mat4f R = rotate(vec3f(3.141592, 0.0, 0.0));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).x, WithinAbs(-1, 0.001));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).y, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).z, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).w, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).x, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).y, WithinAbs(1, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).z, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).w, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).x, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).y, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).z, WithinAbs(-1, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).w, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).x, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).y, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).z, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).w, WithinAbs(1, 0.001));

	R = rotate(vec3f(0.0, 3.141592, 0.0));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).x, WithinAbs(-1, 0.001));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).y, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).z, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).w, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).x, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).y, WithinAbs(-1, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).z, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).w, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).x, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).y, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).z, WithinAbs(1, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).w, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).x, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).y, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).z, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).w, WithinAbs(1, 0.001));

	R = rotate(vec3f(0.0, 0.0, 3.141592));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).x, WithinAbs(1, 0.001));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).y, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).z, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(1, 0, 0, 0)).w, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).x, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).y, WithinAbs(-1, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).z, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 1, 0, 0)).w, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).x, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).y, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).z, WithinAbs(-1, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 1, 0)).w, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).x, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).y, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).z, WithinAbs(0, 0.001));
	CHECK_THAT((R * vec4f(0, 0, 0, 1)).w, WithinAbs(1, 0.001));
}