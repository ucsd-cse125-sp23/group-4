#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"


struct GravityModifierData: ModifierData {
	float gravity;
	GravityModifierData(float gravity) : gravity(gravity) {}
};
class GravityModifier : public Modifier {
	void modify(Modifiable* obj, ModifierData* data) override;
};