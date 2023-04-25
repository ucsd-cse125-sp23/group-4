#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
using Catch::Matchers::WithinAbs;

#include "core/lib.hpp"

bool isAt(PObject* obj, vec3f pos, float epsilon = 0.01f)
{
	return distance_squared(obj->getPos(), pos) < epsilon*epsilon;
}

TEST_CASE("Initialize", "[game]") {
	Environment* environment = new Environment();
	REQUIRE_NOTHROW(initializeLevel(environment));
	REQUIRE_NOTHROW(initializePlayer());
	REQUIRE_NOTHROW(terminateLevel());
}

TEST_CASE("Collision", "[game]") {
	Environment* environment = new Environment();
	environment->addBox(vec3f(-2.0, -3.0, -2.0), vec3f(2.0, -2.0, 2.0));
	REQUIRE_NOTHROW(initializeLevel(environment));
	std::pair<Player*, ControlModifierData*> pair;
	REQUIRE_NOTHROW(pair = initializePlayer());
	Player* player = pair.first;
	ControlModifierData* cmd = pair.second;

	SECTION("No Collision") {
		level->tick();
		CHECK(isAt(player, vec3f(0.0f, -0.01f, 0.0f)));
	}
	SECTION("Pushed Up Collision") {
		player->setPos(vec3f(0.0f, -2.1f, 0.0f));
		level->tick();
		CHECK(isAt(player, vec3f(0.0f, -2.0f, 0.0f)));
	}
	SECTION("Pushed Side Collision") {
		player->setPos(vec3f(2.4f, -3.0f, 0.0f));
		level->tick();
		CHECK(isAt(player, vec3f(2.5f, -3.01f, 0.0f)));
	}


	REQUIRE_NOTHROW(terminateLevel());
}

TEST_CASE("Gravity Modifier", "[modifier]") {
	Environment* environment = new Environment();
	REQUIRE_NOTHROW(initializeLevel(environment));
	std::pair<Player*,ControlModifierData*> pair;
	REQUIRE_NOTHROW(pair = initializePlayer());
	Player* player = pair.first;
	ControlModifierData* cmd = pair.second;

	float p = 0, v = 0;
	SECTION("From origin") {
		for (int i = 0; i < 50; i++)
		{
			CHECK(isAt(player, vec3f(0.0f, p, 0.0f)));
			level->tick();
			v -= 0.01f;
			p += v;
		}
	}
	SECTION("From (1.5f, -0.5f, 1.0f)") {
		player->setPos(vec3f(1.5f, -0.5f, 1.0f));
		p = -0.5f;
		for (int i = 0; i < 50; i++)
		{
			CHECK(isAt(player, vec3f(1.5f, p, 1.0f)));
			level->tick();
			v -= 0.01f;
			p += v;
		}
	}
	SECTION("From origin with ground at -2") {
		environment->addBox(vec3f(-2.0, -4.0, -2.0), vec3f(2.0, -2.0, 2.0));
		for (int i = 0; i < 50; i++)
		{
			CHECK(isAt(player, vec3f(0.0f, p, 0.0f)));
			level->tick();
			v -= 0.01f;
			p += v;
			if (p < -2)
			{
				v = 0;
				p = -2;
			}
		}
	}
	SECTION("From (1.5f, -0.5f, 1.0f) with ground at -2") {
		environment->addBox(vec3f(-2.0, -4.0, -2.0), vec3f(2.0, -2.0, 2.0));
		player->setPos(vec3f(1.5f, -0.5f, 1.0f));
		p = -0.5f;
		for (int i = 0; i < 50; i++)
		{
			CHECK(isAt(player, vec3f(1.5f, p, 1.0f)));
			level->tick();
			v -= 0.01f;
			p += v;
			if (p < -2)
			{
				v = 0;
				p = -2;
			}
		}
	}
	

	REQUIRE_NOTHROW(terminateLevel());
}