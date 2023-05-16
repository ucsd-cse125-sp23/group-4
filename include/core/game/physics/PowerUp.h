#pragma once

#include "core/game/physics/PObject.h"

class PowerUp : public PObject {
public:
	PowerUp(vec3f pos);
	void onTrigger(PObject* other) override;
};
