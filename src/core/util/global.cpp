#include "core/util/global.h"

bool IS_SERVER = true; //Default
bool isServer() {
	return IS_SERVER;
}

PacketHandler* PACKET_HANDLER = nullptr;
int CLIENT_ADD_POBJECT_PACKET_ID = 0;
int CLIENT_UPDATE_POBJECT_PACKET_ID = 1;
int CLIENT_REMOVE_POBJECT_PACKET_ID = 2;
int CLIENT_LEVEL_STATUS_PACKET_ID = 3;

int PLAYER_LAYER = 0;
int ENVIRONMENT_LAYER = 1;
int POWER_LAYER = 2;

CylinderShape* PLAYER_BASE_SHAPE = new CylinderShape(1.0, 0.5);
OffsetShape* PLAYER_BOUNDING_SHAPE = new OffsetShape(PLAYER_BASE_SHAPE, vec3f(0.0f, 0.5f, 0.0f));

Level* level = nullptr;

GameRegistry* GAME_REGISTRY = nullptr;

ControlModifier* CONTROL_MODIFIER = nullptr;
GravityModifier* GRAVITY_MODIFIER = nullptr;
SpeedBoostModifier* SPEEDBOOST_MODIFIER = nullptr;

PObjectType* PLAYER_TYPE = nullptr;