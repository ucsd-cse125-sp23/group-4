#include "core/game/physics/PowerUp.h"
#include "core/util/global.h"

PowerUp::PowerUp(vec3f pos, GlobalEffect* effect) : PObject(POWERUP_BASE_SHAPE, POWER_LAYER), effect(effect)
{
	this->setPos(pos);
}

PowerUp::onTrigger(PObject* other)
{
	effect->apply(level, other);
	this->markRemove();
}
