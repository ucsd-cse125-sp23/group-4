#include "core/game/physics/PowerUp.h"
#include "core/util/global.h"

PowerUp::PowerUp(vec3f pos) : PObject(POWERUP_BASE_SHAPE, POWER_LAYER)
{
	this->setPos(pos);
}

PowerUp::onTrigger(PObject* other)
{
	this->markRemove();
}
