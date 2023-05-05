#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"


struct GravityModifierData: ModifierData {
	float gravity;
	GravityModifierData(float gravity) : gravity(gravity) {}
};
class GravityModifier : public Modifier {
	void modify(Modifiable* obj, ModifierData* data) override;

	virtual size_t size() override {
		return sizeof(GravityModifierData);
	};
	ModifierData* fromBytes(const uint8_t* data) {
		GravityModifierData* ret = new GravityModifierData(0);
		memcpy(ret, data, size());
		return ret;
	}
};