#pragma once

#include "math/vector.h"
#include "math/matrix.h"


class Player {
private:
	static int maxId;
	int id;

	vec3f pos, vel;
public:
	Player();
	~Player();
};