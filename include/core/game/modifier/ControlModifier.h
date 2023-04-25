#pragma once

#include "core/math/vector.h"
#include "core/game/physics/PObject.h"
#include "core/game/modifier/Modifier.h"

struct ControlModifierData : ModifierData {
	vec3f horizontalVel;
	bool doJump;
	float jumpVel;
	ControlModifierData(float jumpVel = 0.5f) : jumpVel(jumpVel), horizontalVel(vec3f(0, 0, 0)), doJump(false) {}
};
class ControlModifier : public Modifier {
	void modify(Modifiable* obj, ModifierData* data) override;
};