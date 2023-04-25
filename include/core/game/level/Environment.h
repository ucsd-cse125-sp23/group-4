#pragma once

#include <vector>

#include "core/game/physics/PObject.h"
#include "core/math/shape/AABShape.h"

class Environment {
private:
	std::vector<PObject*> collisions;
	void addPObject(PObject* obj);
public:
	~Environment();
	const std::vector<PObject*>& getCollisions();
	void addBox(vec3f min, vec3f max, float friction = 0.0f);
};