#pragma once

#include "core/math/shape/CylinderShape.h"
#include "core/math/shape/OffsetShape.h"
#include "core/game/level/Level.h"

#include "core/game/Registry.h"

#include "core/game/modifier/ControlModifier.h"
#include "core/game/modifier/GravityModifier.h"
#include "core/game/modifier/SpeedBoostModifier.h"

#include "core/game/physics/PObjectType.h"
#include "core/game/packet/PacketHandler.h"


extern bool IS_SERVER;
extern bool isServer();

extern PacketHandler* PACKET_HANDLER;
extern int CLIENT_ADD_POBJECT_PACKET_ID;// = 0
extern int CLIENT_UPDATE_POBJECT_PACKET_ID;// = 1
extern int CLIENT_REMOVE_POBJECT_PACKET_ID;// = 2

extern int PLAYER_LAYER;// = 0
extern int ENVIRONMENT_LAYER;// = 1
extern int POWER_LAYER;// = 2

extern CylinderShape* PLAYER_BASE_SHAPE;
extern OffsetShape* PLAYER_BOUNDING_SHAPE;

extern Level* level;

extern GameRegistry* GAME_REGISTRY;

extern ControlModifier* CONTROL_MODIFIER;
extern GravityModifier* GRAVITY_MODIFIER;
extern SpeedBoostModifier* SPEEDBOOST_MODIFIER;

extern PObjectType* PLAYER_TYPE;