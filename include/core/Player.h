#pragma once

#include "core/math/vector.h"
#include "core/math/matrix.h"


class Player {
private:
	static int maxId;
	int id;

	vec3f pos, vel;
public:
	Player();
	~Player();
};