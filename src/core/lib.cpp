#include "core/lib.hpp"

#include "core/util/global.h"
//#include "core/game/physics/PObjectType.h"


void initializeLib(bool isServer) {
	IS_SERVER = isServer;

	GAME_REGISTRY = new GameRegistry();

	GAME_REGISTRY->MODIFIER_REGISTRY.registerType("control", CONTROL_MODIFIER = new ControlModifier());
	GAME_REGISTRY->MODIFIER_REGISTRY.registerType("gravity", GRAVITY_MODIFIER = new GravityModifier());
	GAME_REGISTRY->MODIFIER_REGISTRY.registerType("speedboost", SPEEDBOOST_MODIFIER = new SpeedBoostModifier());

	//GAME_REGISTRY->POBJECT_REGISTRY.registerType("player", PLAYER_TYPE = new PObjectType("player", []()-> PObject* {return new Player(); }));
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
