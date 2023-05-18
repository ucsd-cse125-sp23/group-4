#include "core/game/modifier/ControlModifier.h"
#include "core/game/modifier/GravityModifier.h"
#include "core/game/modifier/SpeedBoostModifier.h"
#include "core/game/modifier/TimedModifier.h"
#include "core/util/global.h"


void TimedModifier::modify(Modifiable* obj, ModifierData* data)
{
	TimedModifierData* cData = static_cast<TimedModifierData*>(data);
	if (level->getAge() >= cData->expire)
		cData->markedRemove = true;
}

void SpeedBoostModifier::timedModify(Modifiable* obj, ModifierData* data)
{
	if (PObject* pObj = dynamic_cast<PObject*>(obj))
	{
		SpeedBoostModifierData* cData = static_cast<SpeedBoostModifierData*>(data);
		pObj->addPos(cData->factorIncrease * pObj->vel * vec3f(1,0,1));
	}
}

void GravityModifier::modify(Modifiable* obj, ModifierData* data)
{
	if (PObject* pObj = dynamic_cast<PObject*>(obj))
		pObj->vel += vec3f(0.0f, -static_cast<GravityModifierData*>(data)->gravity, 0.0f);
}

void ControlModifier::modify(Modifiable* obj, ModifierData* data)
{
	if (PObject* pObj = dynamic_cast<PObject*>(obj))
	{
		ControlModifierData* cData = static_cast<ControlModifierData*>(data);
		vec3f dv = clampBySign(cData->horizontalVel, cData->horizontalVel - pObj->vel) * 0.6f;
		pObj->vel += vec3f(dv.x, 0.0f, dv.z);
		if (pObj->onGround && cData->doJump)
		{
			pObj->vel.y = cData->jumpVel;
			pObj->onGround = false;
		}
	}
}