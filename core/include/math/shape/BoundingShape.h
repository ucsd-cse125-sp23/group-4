#pragma once

#include "math/vector.h"
#include "math/matrix.h"

class ConvexShape;

class BoundingShape {
public:
	virtual bool collides(const BoundingShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const = 0;
	virtual bool collides(const ConvexShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const = 0;
	virtual vec3f mtv(const BoundingShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const = 0;
	virtual vec3f mtv(const ConvexShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const = 0;

	virtual const ConvexShape** seperate() const = 0;
	virtual const size_t count() const = 0;
};