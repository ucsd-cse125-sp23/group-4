#pragma once

#include "core/math/Ray.h"
#include "core/math/vector.h"
#include "core/math/matrix.h"

class ConvexShape;

class BoundingShape {
public:
	virtual bool collides(const BoundingShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const = 0;
	virtual bool collides(const ConvexShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const = 0;
	virtual vec4f mtv(const BoundingShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const = 0;
	virtual vec4f mtv(const ConvexShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const = 0;

	virtual bool contains(const vec3f& point, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const = 0;
	virtual float intersects(const Ray& ray, const mat4f& thisMtx = mat4f::identity()) const = 0;

	virtual const ConvexShape** seperate() const = 0;
	virtual const size_t count() const = 0;
};