#include "core/lib.hpp"

#include "core/util/global.h"


void initializeLib() {
	CONTROL_MODIFIER = new ControlModifier();
	GRAVITY_MODIFIER = new GravityModifier();
	SPEEDBOOST_MODIFIER = new SpeedBoostModifier();

	Modifier::registerModifier("control", CONTROL_MODIFIER);
	Modifier::registerModifier("gravity", GRAVITY_MODIFIER);
	Modifier::registerModifier("speedboost", SPEEDBOOST_MODIFIER);
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
