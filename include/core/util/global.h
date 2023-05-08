#pragma once

#include "core/math/shape/CylinderShape.h"
#include "core/math/shape/OffsetShape.h"
#include "core/game/level/Level.h"

#include "core/game/Registry.h"

#include "core/game/modifier/ControlModifier.h"
#include "core/game/modifier/GravityModifier.h"
#include "core/game/modifier/SpeedBoostModifier.h"


extern bool IS_SERVER;
extern bool isServer() {
	return IS_SERVER;
}

extern int PLAYER_LAYER;// = 0;
extern int ENVIRONMENT_LAYER;// = 1;
extern int POWER_LAYER;// = 2;

extern CylinderShape* PLAYER_BASE_SHAPE;// = new CylinderShape(1.0, 0.5);
extern OffsetShape* PLAYER_BOUNDING_SHAPE;// = new OffsetShape(PLAYER_BASE_SHAPE, vec3f(0, 1, 0));

extern Level* level;// = nullptr;

extern GameRegistry* GAME_REGISTRY;

extern ControlModifier* CONTROL_MODIFIER;// = new ControlModifier();
extern GravityModifier* GRAVITY_MODIFIER;// = new GravityModifier();
extern SpeedBoostModifier* SPEEDBOOST_MODIFIER;// = new SpeedBoostModifier();

extern PObjectType* PLAYER_TYPE;