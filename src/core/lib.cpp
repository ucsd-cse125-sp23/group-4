#include "core/lib.hpp"

#include "core/util/global.h"
#include "core/game/physics/PObjectType.h"

#include "core/game/packet/CAddPObjectPacket.h"
#include "core/game/packet/CRemovePObjectPacket.h"
#include "core/game/packet/CUpdatePObjectPacket.h"


void initializeLib(bool isServer) {
	IS_SERVER = isServer;

	GAME_REGISTRY = new GameRegistry();

	GAME_REGISTRY->MODIFIER_REGISTRY.registerType("control", CONTROL_MODIFIER = new ControlModifier());
	GAME_REGISTRY->MODIFIER_REGISTRY.registerType("gravity", GRAVITY_MODIFIER = new GravityModifier());
	GAME_REGISTRY->MODIFIER_REGISTRY.registerType("speedboost", SPEEDBOOST_MODIFIER = new SpeedBoostModifier());

	GAME_REGISTRY->POBJECT_REGISTRY.registerType("player", PLAYER_TYPE = new PObjectType("player", []()-> PObject* {return new Player(); }));

	PACKET_HANDLER = new PacketHandler();
	PACKET_HANDLER->registerPacket(CLIENT_ADD_POBJECT_PACKET_ID, NetworkDirection::CLIENT_BOUND, {
			CAddPObjectPacket::handle,
			CAddPObjectPacket::fromBytes,
			CAddPObjectPacket::toBytes
		}
	);
	PACKET_HANDLER->registerPacket(CLIENT_REMOVE_POBJECT_PACKET_ID, NetworkDirection::CLIENT_BOUND, {
			CRemovePObjectPacket::handle,
			CRemovePObjectPacket::fromBytes,
			CRemovePObjectPacket::toBytes
		}
	);
	PACKET_HANDLER->registerPacket(CLIENT_UPDATE_POBJECT_PACKET_ID, NetworkDirection::CLIENT_BOUND, {
			CUpdatePObjectPacket::handle,
			CUpdatePObjectPacket::fromBytes,
			CUpdatePObjectPacket::toBytes
		}
	);
}

void initializeLevel(Environment* environment) {
	level = new Level(environment);
	level->setCollisionType(CollisionType::COLLISION, PLAYER_LAYER, ENVIRONMENT_LAYER);
}

std::pair<Player*, ControlModifierData*> initializePlayer() {
	Player* player = new Player();
	ControlModifierData* controlData = new ControlModifierData();
	player->addModifierInstance(new ModifierInstance(CONTROL_MODIFIER, controlData));
	level->addPObject(player);
	return std::make_pair(player, controlData);
}

void terminateLevel() {
	delete level;
}

void terminateLib() {
	//delete GAME_REGISTRY;
	delete PACKET_HANDLER;
}