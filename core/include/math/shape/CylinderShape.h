#pragma once

#include "math/shape/ConvexShape.h"


int sign(float f) {
	if (f == 0)
		return 0;
	else if (f > 0)
		return 1;
	return 0;
}
class CylinderShape : public ConvexShape {
private:
	static const vec3f up;
	float height, radius;
protected:
	vec4f furthestPoint(vec3f dir) const override {
		float d = dot(up, dir);
		vec3f rad = dir - d * up;
		return vec4f(normalize(rad) * radius + up * (sign(d) * height / 2), 1.0f);
	};
public:
	CylinderShape(float height, float radius) : height(height), radius(radius) {}
};

const vec3f CylinderShape::up = vec3f(0, 1, 0);