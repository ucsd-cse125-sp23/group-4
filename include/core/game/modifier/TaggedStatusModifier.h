#pragma once

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObject.h"


struct TaggedStatusModifierData : ModifierData {
	int cooldown;
	int ticksIt;
	bool isIt;
	TaggedStatusModifierData(bool isIt) : cooldown(0), ticksIt(0), isIt(isIt) {}
	TaggedStatusModifierData() : TaggedStatusModifierData(false) {}
};
class TaggedStatusModifier : public Modifier {
	void modify(Modifiable* obj, ModifierData* data) override;
};