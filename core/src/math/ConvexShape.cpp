#pragma once

#include <limits>
#include <vector>
#include "math/shape/ConvexShape.h"
#include "math/shape/Simplex.h"

//https://blog.winter.dev/2020/gjk-algorithm/
//https://blog.winter.dev/2020/epa-algorithm/
vec3f ConvexShape::support(const ConvexShape* shape0, const mat4f& mtx0, const mat3f& iMtx0,
						   const ConvexShape* shape1, const mat4f& mtx1, const mat3f& iMtx1,
						   vec3f dir)
{
	return vec3f(mtx0 * shape0->furthestPoint(iMtx0 * dir) - mtx1 * shape1->furthestPoint(iMtx1 * (-dir)));
}

bool sameDir(const vec3f& dir, const vec3f& ao)
{
	return dot(dir,ao) > 0;
}
bool line(Simplex& pts, vec3f& dir)
{
	vec3f a = pts[0];
	vec3f b = pts[1];

	vec3f ab = b - a;
	vec3f ao = -a;

	if (sameDir(ab, ao)) {
		dir = cross(cross(ab, ao), ab);
		if (length_squared(dir) == 0)
			return true;
	}
	else {
		pts = { a };
		dir = ao;
	}

	return false;
}
bool triangle(Simplex& pts, vec3f& dir)
{
	vec3f a = pts[0];
	vec3f b = pts[1];
	vec3f c = pts[2];

	vec3f ab = b - a;
	vec3f ac = c - a;
	vec3f ao =   - a;
 
	vec3f abc = cross(ab, ac);
 
	if (sameDir(cross(abc, ac), ao)) {
		if (sameDir(ac, ao)) {
			pts = { a, c };
			dir = cross(cross(ac, ao), ac);
		}

		else {
			return line(pts = { a, b }, dir);
		}
	}
 
	else {
		if (sameDir(cross(ab, abc), ao)) {
			return line(pts = { a, b }, dir);
		}

		else {
			if (sameDir(abc, ao)) {
				dir = abc;
			}

			else {
				pts = { a, c, b };
				dir = -abc;
			}
		}
	}

	return false;
}
bool tetrahedron(Simplex& pts, vec3f& dir)
{
	vec3f a = pts[0];
	vec3f b = pts[1];
	vec3f c = pts[2];
	vec3f d = pts[3];

	vec3f ab = b - a;
	vec3f ac = c - a;
	vec3f ad = d - a;
	vec3f ao = -a;

	vec3f abc = cross(ab,ac);
	vec3f acd = cross(ac,ad);
	vec3f adb = cross(ad,ab);

	if (sameDir(abc, ao)) {
		return triangle(pts = { a, b, c }, dir);
	}

	if (sameDir(acd, ao)) {
		return triangle(pts = { a, c, d }, dir);
	}

	if (sameDir(adb, ao)) {
		return triangle(pts = { a, d, b }, dir);
	}

	return true;
}
bool nextSimplex(Simplex& pts, vec3f& direction)
{
	switch (pts.size()) {
	case 2: return line(pts, direction);
	case 3: return triangle(pts, direction);
	case 4: return tetrahedron(pts, direction);
	}
	return false;
}

bool ConvexShape::collides(const ConvexShape* other, const mat4f& thisMtx, const mat3f& thisIMtx, const mat4f& otherMtx, const mat3f& otherIMtx) const
{
	vec3f support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx, otherIMtx, { 1,0,0 });
	if (length_squared(support) == 0)
		return true;
	Simplex pts;
	pts.push(support);

	vec3f dir = -support;
	while (true) {
		support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx, otherIMtx, dir);
		if (dot(support,dir) <= 0)
			return false;
		pts.push(support);
		if (nextSimplex(pts, dir)) {
			return true;
		}
	}
}
bool ConvexShape::collides(const ConvexShape* other, const mat4f& thisMtx, const mat4f& otherMtx) const
{
	mat3f thisIMtx = mat3f(thisMtx);
	if (thisIMtx != mat3f::identity())
		thisIMtx = inverse(thisIMtx);
	mat3f otherIMtx = mat3f(otherMtx);
	if (otherIMtx != mat3f::identity())
		otherIMtx = inverse(otherIMtx);

	return this->collides(other, thisMtx, thisIMtx, otherMtx, otherIMtx);
}

std::pair<std::vector<vec4f>, size_t> getFaceNormals(const std::vector<vec3f>& polytope, const std::vector<size_t>& faces)
{
	std::vector<vec4f> normals;
	size_t minTriangle = 0;
	float  minDistance = FLT_MAX;

	for (size_t i = 0; i < faces.size(); i += 3) {
		vec3f a = polytope[faces[i]];
		vec3f b = polytope[faces[i + 1]];
		vec3f c = polytope[faces[i + 2]];

		vec3f normal = normalize(cross(b - a, c - a));
		float distance = dot(normal, a);

		if (distance < 0) {
			normal = -normal;
			distance *= -1;
		}

		normals.push_back(vec4f(normal, distance));

		if (distance < minDistance) {
			minTriangle = i / 3;
			minDistance = distance;
		}
	}

	return { normals, minTriangle };
}
void addIfUniqueEdge(
	std::vector<std::pair<size_t, size_t>>& edges,
	const std::vector<size_t>& faces,
	size_t a,
	size_t b)
{
	auto reverse = std::find(              //      0--<--3
		edges.begin(),                     //     / \ B /   A: 2-0
		edges.end(),                       //    / A \ /    B: 0-2
		std::make_pair(faces[b], faces[a]) //   1-->--2
	);

	if (reverse != edges.end()) {
		edges.erase(reverse);
	}
	else {
		edges.emplace_back(faces[a], faces[b]);
	}
}
vec4f ConvexShape::mtv(const ConvexShape* other, const mat4f& thisMtx, const mat3f& thisIMtx, const mat4f& otherMtx, const mat3f& otherIMtx) const
{
	//GJK
	vec3f support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx, otherIMtx, { 1,0,0 });
	Simplex pts;
	pts.push(support);
	if (length_squared(support) != 0)
	{
		vec3f dir = -support;
		while (true) {
			support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx, otherIMtx, dir);
			if (dot(support, dir) <= 0)
				return vec4f(0, 0, 0, 0);
			pts.push(support);
			if (nextSimplex(pts, dir)) {
				break;
			}
		}
	}

	//Fix simplex
	if (pts.size() == 2)
	{
		vec3f l = normalize(pts[0] - pts[1]);
		vec3f p = vec3f(-l.y, l.x, l.z);
		pts.push(ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx, otherIMtx, p - dot(l, p) * l));
	}
	if (pts.size() == 3)
	{
		vec3f ba = pts[1] - pts[0];
		vec3f ca = pts[2] - pts[0];
		pts.push(ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx, otherIMtx, cross(ba, ca)));
	}

	//EPA
	//Safety loop escape condition
	float scale = 0.001f;
	float gMin = FLT_MAX;
	vec4f gNorm = vec4f(0, 0, 0, 0);


	std::vector<vec3f> polytope(pts.begin(), pts.end());
	std::vector<size_t> faces = {
		0, 1, 2,
		0, 3, 1,
		0, 2, 3,
		1, 3, 2
	};
	std::pair<std::vector<vec4f>, size_t> pair = getFaceNormals(polytope, faces);
	std::vector<vec4f> normals = pair.first;
	size_t minFace = pair.second;

	vec3f minNormal;
	float minDistance = FLT_MAX;

	while (minDistance == FLT_MAX) {
		minNormal = vec3f(normals[minFace]);
		minDistance = normals[minFace].w;
		
		vec3f support = ConvexShape::support(this, thisMtx, thisIMtx, other, otherMtx, otherIMtx, minNormal);
		float sDistance = dot(minNormal, support);

		float diff = abs(sDistance - minDistance);
		if (diff > scale) {
			if (diff < gMin)
			{
				gMin = diff;
				scale = 0.001f;
				gNorm = vec4f(-minNormal, minDistance);
			}
			else
				scale *= 2.0f;


			minDistance = FLT_MAX;
			std::vector<std::pair<size_t, size_t>> uniqueEdges;

			for (size_t i = 0; i < normals.size(); i++) {
				if (sameDir(normals[i], support)) {
					size_t f = i * 3;

					addIfUniqueEdge(uniqueEdges, faces, f, f + 1);
					addIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
					addIfUniqueEdge(uniqueEdges, faces, f + 2, f);

					faces[f + 2] = faces.back(); faces.pop_back();
					faces[f + 1] = faces.back(); faces.pop_back();
					faces[f] = faces.back(); faces.pop_back();

					normals[i] = normals.back(); normals.pop_back();

					i--;
				}
			}
			std::vector<size_t> newFaces;
			for (auto pair : uniqueEdges) {
				newFaces.push_back(pair.first);
				newFaces.push_back(pair.second);
				newFaces.push_back(polytope.size());
			}
			polytope.push_back(support);

			std::pair<std::vector<vec4f>, size_t> pair = getFaceNormals(polytope, newFaces);
			std::vector<vec4f> newNormals = pair.first;
			size_t newMinFace = pair.second;
			float oldMinDistance = FLT_MAX;
			for (size_t i = 0; i < normals.size(); i++) {
				if (normals[i].w < oldMinDistance) {
					oldMinDistance = normals[i].w;
					minFace = i;
				}
			}

			if (newNormals[newMinFace].w < oldMinDistance) {
				minFace = newMinFace + normals.size();
			}

			faces.insert(faces.end(), newFaces.begin(), newFaces.end());
			normals.insert(normals.end(), newNormals.begin(), newNormals.end());
		}
	}

	if (gNorm.w < minDistance)
		return gNorm;
	return vec4f(-minNormal, minDistance);
}
vec4f ConvexShape::mtv(const ConvexShape* other, const mat4f& thisMtx, const mat4f& otherMtx) const
{
	mat3f thisIMtx = mat3f(thisMtx);
	if (thisIMtx != mat3f::identity())
		thisIMtx = inverse(thisIMtx);
	mat3f otherIMtx = mat3f(otherMtx);
	if (otherIMtx != mat3f::identity())
		otherIMtx = inverse(otherIMtx);

	return this->mtv(other, thisMtx, thisIMtx, otherMtx, otherIMtx);
}

bool ConvexShape::collides(const BoundingShape* other, const mat4f& thisMtx, const mat4f& otherMtx) const
{
	mat3f thisIMtx = mat3f(thisMtx);
	if (thisIMtx != mat3f::identity())
		thisIMtx = inverse(thisIMtx);
	mat3f otherIMtx = mat3f(otherMtx);
	if (otherIMtx != mat3f::identity())
		otherIMtx = inverse(otherIMtx);

	const ConvexShape** ptr = other->seperate();
	for (int i = 0; i < other->count(); i++)
	{
		if (ptr[i]->collides(this, otherMtx, otherIMtx, thisMtx, thisIMtx))
			return true;
	}
	return false;
}

vec4f ConvexShape::mtv(const BoundingShape* other, const mat4f& thisMtx, const mat4f& otherMtx) const
{
	mat3f thisIMtx = mat3f(thisMtx);
	if (thisIMtx != mat3f::identity())
		thisIMtx = inverse(thisIMtx);
	mat3f otherIMtx = mat3f(otherMtx);
	if (otherIMtx != mat3f::identity())
		otherIMtx = inverse(otherIMtx);

	vec4f min = vec4f(0.0f, 0.0f, 0.0f, 0.0f);
	float minLS = std::numeric_limits<float>::max();
	const ConvexShape** ptr = other->seperate();
	for (int i = 0; i < other->count(); i++)
	{
		vec4f v = ptr[i]->mtv(this, otherMtx, otherIMtx, thisMtx, thisIMtx);
		if (v.w > 0 && minLS > v.w)
		{
			min = v;
			minLS = v.w;
		}
	}
	return -min;
}