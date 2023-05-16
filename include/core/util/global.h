#pragma once

#include "core/game/event/EventManager.h"
#include "core/game/level/Level.h"
#include "core/game/modifier/ControlModifier.h"
#include "core/game/modifier/GravityModifier.h"
#include "core/game/modifier/SpeedBoostModifier.h"
#include "core/game/modifier/AttractModifier.h"
#include "core/game/modifier/FreezeModifier.h"
#include "core/game/modifier/TaggedStatusModifier.h"
#include "core/math/shape/SphereShape.h"
#include "core/math/shape/CylinderShape.h"
#include "core/math/shape/OffsetShape.h"
#include "core/game/effect/GlobalEffect.h"

extern int TAG_COOLDOWN;
extern float JUMP_VELOCITY;
extern float GRAVITY_STRENGTH;

extern int PLAYER_LAYER;       // = 0;
extern int ENVIRONMENT_LAYER;  // = 1;
extern int POWER_LAYER;        // = 2;

extern SphereShape* POWERUP_BASE_SHAPE;
extern CylinderShape* PLAYER_BASE_SHAPE;
extern OffsetShape* PLAYER_BOUNDING_SHAPE;

extern Level* level;

extern ControlModifier* CONTROL_MODIFIER;
extern GravityModifier* GRAVITY_MODIFIER;
extern TaggedStatusModifier* TAGGED_STATUS_MODIFIER;

extern SpeedBoostModifier* SPEEDBOOST_MODIFIER;
extern AttractModifier* ATTRACT_MODIFIER;
extern FreezeModifier* FREEZE_MODIFIER;


extern GlobalEffect* SPEEDBOOST_EFFECT;
