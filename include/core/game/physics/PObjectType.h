#pragma once

#include <functional>
#include <string>
#include "core/game/physics/PObject.h"

class PObjectType {
public:
	const std::string key;
	const std::function<PObject*()> instansiate;
	PObjectType(std::string key, std::function<PObject*()> func);
};