#include "core/lib.hpp"

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
