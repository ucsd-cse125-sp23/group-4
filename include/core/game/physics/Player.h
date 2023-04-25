#pragma once

#include "core/math/vector.h"
#include "core/game/physics/PObject.h"


class Player : public PObject {
private:
	static uint32_t maxId;
public:
	vec3f look;
	uint32_t pid;
	Player();
	~Player();
	void tick();
};