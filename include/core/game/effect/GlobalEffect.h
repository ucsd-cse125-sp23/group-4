#pragma once

#include <functional>
#include <vector>
#include "core/game/physics/PObject.h"

struct GlobalEffect {
private:
	std::function<void(Level*, std::vector<PObject*>)> applyTo;
public:
	GlobalEffect(std::function<void(Level*, std::vector<PObject*>)> f);
	void apply(Level* level, PObject* target);
	void apply(Level* level, std::vector<PObject*> targets);
};
