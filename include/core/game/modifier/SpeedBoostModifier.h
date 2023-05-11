#pragma once

#include "core/math/vector.h"
#include "core/game/physics/PObject.h"
#include "core/game/modifier/TimedModifier.h"

struct SpeedBoostModifierData : TimedModifierData {
	float factorIncrease;
	SpeedBoostModifierData(unsigned long long expire, float factorIncrease) : TimedModifierData(expire), factorIncrease(factorIncrease) {}
};
class SpeedBoostModifier : public TimedModifier {
	SpeedBoostModifier();
	void timedModify(Modifiable* obj, ModifierData* data) override;
};