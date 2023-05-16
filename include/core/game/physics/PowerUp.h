#pragma once

#include "core/game/effect/GlobalEffect.h"
#include "core/game/physics/PObject.h"

class PowerUp : public PObject {
private:
	const GlobalEffect* effect;
public:
	PowerUp(vec3f pos, GlobalEffect* effect);
	void onTrigger(const PObject* other) override;
};
