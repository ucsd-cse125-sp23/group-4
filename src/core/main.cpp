#include <iostream>

#include "core/game/modifier/Modifier.h"
#include "core/game/modifier/ModifierInstance.h"
#include "core/game/physics/Player.h"
#include "core/math/shape/OffsetShape.h"
#include "core/util/global.h"

#include "core/game/modifier/GravityModifier.h"

int main(int argc, char* argv[])
{
	std::cout << "Hello world!" << std::endl;

	OffsetShape* test = new OffsetShape(PLAYER_BASE_SHAPE, vec3f(0, 1, 0));
	/*
	PObject* player = new Player();
	std::cout << player->getPos() << " " << player->vel << std::endl;
	for (int i = 0; i < 10; i++)
	{
		player->tick();
		std::cout << player->getPos() << " " << player->vel << std::endl;
	}
	*/

	std::cout << "End" << std::endl;
}