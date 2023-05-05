#pragma once

#include <any>

#include "core/game/modifier/Modifier.h"

class ModifierInstance {
private:
	Modifier* modifier;
	ModifierData* data;
public:
	ModifierInstance(Modifier* modifier, ModifierData* data) : modifier(modifier), data(data) { }
	~ModifierInstance() {
		delete data;
	}
	Modifier* getModifier() {
		return modifier;
	}
	ModifierData* get() {
		return data;
	}
};