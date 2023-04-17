#pragma once

#include "math/shape/ConvexMeshShape.h"


class AABShape : public ConvexMeshShape {
public:
	AABShape(vec3f min, vec3f max) :
		ConvexMeshShape({ vec3f(min.x,min.y,min.z),
						  vec3f(min.x,min.y,max.z),
						  vec3f(min.x,max.y,min.z),
						  vec3f(min.x,max.y,max.z),
						  vec3f(max.x,min.y,min.z),
						  vec3f(max.x,min.y,max.z),
						  vec3f(max.x,max.y,min.z),
						  vec3f(max.x,max.y,max.z)})
	{}
};