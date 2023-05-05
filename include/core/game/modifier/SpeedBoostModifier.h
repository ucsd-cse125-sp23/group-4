#pragma once

#include "core/math/vector.h"
#include "core/game/physics/PObject.h"
#include "core/game/modifier/TimedModifier.h"

struct SpeedBoostModifierData : TimedModifierData {
	float factorIncrease;
	SpeedBoostModifierData(unsigned long long expire, float factorIncrease) : TimedModifierData(expire), factorIncrease(factorIncrease) {}
};
class SpeedBoostModifier : public TimedModifier {
	void timedModify(Modifiable* obj, ModifierData* data) override;

	virtual size_t size() override {
		return sizeof(SpeedBoostModifierData);
	};
	ModifierData* fromBytes(const uint8_t* data) {
		SpeedBoostModifierData* ret = new SpeedBoostModifierData(0,0);
		memcpy(ret, data, size());
		return ret;
	}
};