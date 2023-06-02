#pragma once

#include "core/game/effect/ContextAwareGlobalEffect.h"
#include "core/game/effect/StaticGlobalEffect.h"
#include "core/game/event/EventManager.h"
#include "core/game/level/Level.h"
#include "core/game/modifier/AttractModifier.h"
#include "core/game/modifier/ControlModifier.h"
#include "core/game/modifier/FreezeModifier.h"
#include "core/game/modifier/GravityModifier.h"
#include "core/game/modifier/SpeedBoostModifier.h"
#include "core/game/modifier/TaggedStatusModifier.h"
#include "core/math/shape/CapsuleShape.h"
#include "core/math/shape/OffsetShape.h"
#include "core/math/shape/SphereShape.h"

extern int TAG_COOLDOWN;
extern float JUMP_VELOCITY;
extern float GRAVITY_STRENGTH;

extern int PLAYER_LAYER;       // = 0;
extern int ENVIRONMENT_LAYER;  // = 1;
extern int POWER_LAYER;        // = 2;

extern SphereShape* POWERUP_BASE_SHAPE;
extern CapsuleShape* PLAYER_BASE_SHAPE;
extern OffsetShape* PLAYER_BOUNDING_SHAPE;

extern Level* level;

extern ControlModifier* CONTROL_MODIFIER;
extern GravityModifier* GRAVITY_MODIFIER;
extern TaggedStatusModifier* TAGGED_STATUS_MODIFIER;

extern SpeedBoostModifier* SPEEDBOOST_MODIFIER;
extern AttractModifier* ATTRACT_MODIFIER;
extern FreezeModifier* FREEZE_MODIFIER;

extern GlobalEffect* SPEEDBOOST_EFFECT;
extern GlobalEffect* SLOWDOWN_EFFECT;
extern GlobalEffect* FREEZE_EFFECT;
extern GlobalEffect* REVERSE_EFFECT;
extern GlobalEffect* LAUNCH_EFFECT;
extern GlobalEffect* SLOW_FALL_EFFECT;
extern GlobalEffect* FAST_FALL_EFFECT;

extern GlobalEffect* SWAP_POSITIONS_EFFECT;

/*Good*/
extern GlobalEffect* SPEEDBOOST_SELF_EFFECT;
extern GlobalEffect* REPELL_TAGGER_FROM_SELF;
extern GlobalEffect* ATTRACT_OTHER_TO_TAGGERS_EFFECT;
extern GlobalEffect* FREEZE_OTHER_EFFECT;
extern GlobalEffect* SLOWDOWN_OTHER_EFFECT;
extern GlobalEffect* REVERSE_OTHER_EFFECT;

/*Ok*/
extern GlobalEffect* SPEEDBOOST_SELF_TAG_STATUS_EFFECT;
extern GlobalEffect* FREEZE_NOT_SELF_TAG_STATUS_EFFECT;
extern GlobalEffect* SLOWDOWN_NOT_SELF_TAG_STATUS_EFFECT;
extern GlobalEffect* REVERSE_NOT_SELF_TAG_STATUS_EFFECT;

/*Neutral*/
extern GlobalEffect* LAUNCH_OTHER_EFFECT;
extern GlobalEffect* LAUNCH_NOT_SELF_TAG_STATUS_EFFECT;
extern GlobalEffect* SLOW_FALL_OTHER_EFFECT;
extern GlobalEffect* FAST_FALL_OTHER_EFFECT;
extern GlobalEffect* SWAP_POSITIONS_OTHER_EFFECT;
extern GlobalEffect* SWAP_POSITIONS_ALL_EFFECT;
