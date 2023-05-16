#pragma once

#include <vector>
#include "core/game/physics/PObject.h"
#include "core/game/level/Level.h"

struct GlobalEffect {
public:
	void apply(Level* level, PObject* target);
	virtual void apply(Level* level, std::vector<PObject*> targets) = 0;
};
