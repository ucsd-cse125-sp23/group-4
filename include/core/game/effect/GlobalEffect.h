#pragma once

#include <functional>
#include <vector>
#include "core/game/physics/PObject.h"

template <typename U>
struct Effect : private U {
	std::function<void(Level*, std::vector<PObject*>)> applyTo;
	Effect(U u);
	void apply(Level* level, PObject* target);
	void apply(Level* level, std::vector<PObject*> targets);
};


extern Effect* SPEEDBOOST_EFFECT;