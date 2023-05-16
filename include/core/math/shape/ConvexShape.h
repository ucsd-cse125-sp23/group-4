#pragma once

#include "core/math/shape/BoundingShape.h"

//https://blog.winter.dev/2020/gjk-algorithm/
class ConvexShape : public BoundingShape {
private:
	const ConvexShape** self;
protected:
	vec3f static support(const ConvexShape* shape0, const mat4f& mtx0, const mat3f& iMtx0, const ConvexShape* shape1, const mat4f& mtx1, const mat3f& iMtx1, vec3f dir);
	
	bool collides(const ConvexShape* other, const mat4f& thisMtx, const mat3f& thisIMtx, const mat4f& otherMtx, const mat3f& otherIMtx) const;
	vec4f mtv(const ConvexShape* other, const mat4f& thisMtx, const mat3f& thisIMtx, const mat4f& otherMtx, const mat3f& otherIMtx) const;
public:
	ConvexShape() {
		self = (const ConvexShape**)malloc(sizeof(const ConvexShape**));
		*self = this;
	}
	~ConvexShape() {
		free(self);
	}
	bool collides(const BoundingShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const override;
	bool collides(const ConvexShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const override;
	vec4f mtv(const BoundingShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const override;
	vec4f mtv(const ConvexShape* other, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const override;

	bool contains(const vec3f& point, const mat4f& thisMtx = mat4f::identity(), const mat4f& otherMtx = mat4f::identity()) const override;
	float intersects(const Ray& ray, const mat4f& thisMtx = mat4f::identity()) const override;

	const ConvexShape** seperate() const override { return self; }
	const size_t count() const override { return 1; }

	/**
	 * @param dir (not normalized)
	 * @return homogeneous vector of the furthest point of this shape in the projected on to dir
	 */
	virtual vec4f furthestPoint(vec3f dir) const = 0;
};