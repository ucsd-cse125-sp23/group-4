#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
using Catch::Matchers::WithinAbs;

#include "core/lib.hpp"
#include <string>


Environment* testEnvironment()
{
	/*
		^^^^^^^^^^
		|        |
		|        |
		|        |
		|        |
		|        |
		\        /
		 \      /
		  \    /
		   \  /
		    \/
	*/
	Environment* environment = new Environment();
	environment->addBox(vec3f(-5, -5, -5), vec3f(5, 0, 5));
	environment->addConvex({vec3f(-5,0,0), vec3f(0,0,-5), vec3f(-5,0,-5), vec3f(-5,5,0), vec3f(0,5,-5), vec3f(-5,5,-5)});
	environment->addConvex({ vec3f(-5,0,0), vec3f(0,0,5), vec3f(-5,0,5), vec3f(-5,5,0), vec3f(0,5,5), vec3f(-5,5,5) });
	environment->addBox(vec3f(0, 0, -6), vec3f(5, 5, -5));
	environment->addBox(vec3f(0, 0, 5), vec3f(5, 5, 6));

	environment->addConvex({ vec3f(5,0,-5), vec3f(10,0,-5), vec3f(10,3,-5), vec3f(5,0,5), vec3f(10,0,5), vec3f(10,3,5) });
	environment->addBox(vec3f(10, 2, -5), vec3f(15, 3, 5));
	
	return environment;
}

Environment* jumpEnvironment()
{
	/*
		__________
		|44444444|
		|44444444|
		|33333333|
		|33333333|
		|22222222|
		|22222222|
		|11111111|
		|11111111|
		|        |
		|________|
	*/
	Environment* environment = new Environment();
	environment->addBox(vec3f(-5, -5, -5), vec3f(5, 0, 5));
	environment->addBox(vec3f(-6, 0, -6), vec3f(-5, 10, 6));
	environment->addBox(vec3f(-6, 0, -6), vec3f(6, 10, -5));
	environment->addBox(vec3f(5, 0, -6), vec3f(6, 10, 6));
	environment->addBox(vec3f(-6, 0, 5), vec3f(6, 10, 6));

	environment->addBox(vec3f(-3, 0, -5), vec3f(-1, 1, 5));
	environment->addBox(vec3f(-1, 0, -5), vec3f(1, 2, 5));
	environment->addBox(vec3f(1, 0, -5), vec3f(3, 3, 5));
	environment->addBox(vec3f(3, 0, -5), vec3f(5, 4, 5));


	return environment;
}


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

void arrivesAt(vec3f pos, PObject* obj, size_t maxTicks = 1000, float epsilon = 0.25f) {
	size_t ticks = 0;
	do {
		level->tick();
		ticks++;
		if (ticks > maxTicks)
		{
			INFO("Test exceeded maximum number of ticks");
			INFO("Reached (" + std::to_string(obj->getPos().x) + ", " + std::to_string(obj->getPos().y) + ", " + std::to_string(obj->getPos().z) + ")");
			CHECK(false);
			return;
		}
	} while (distance_squared(pos, obj->getPos()) > epsilon * epsilon);
	CHECK(true);
}

void playerArrivesAt(std::initializer_list<vec3f> poss, std::initializer_list<vec3f> vels, Player* player, ControlModifierData* cmd, size_t maxTicksPerStep = 1000, float epsilon = 0.25f) {
	if (poss.size() != vels.size())
	{
		INFO("Position list and Velocity list are not the same length");
		return;
	}
	auto posIte = poss.begin();
	auto velIte = vels.begin();
	for (int i = 0; i < poss.size(); i++)
	{
		size_t ticks = 0;
		cmd->horizontalVel = *velIte;
		do {
			level->tick();
			ticks++;
			if (ticks > maxTicksPerStep)
			{
				INFO("Test exceeded maximum number of ticks at step " + std::to_string(i));
				INFO("Reached (" + std::to_string(player->getPos().x) + ", " + std::to_string(player->getPos().y) + ", " + std::to_string(player->getPos().z) + ")");
				CHECK(false);
				return;
			}
		} while (distance_squared(*posIte, player->getPos()) > epsilon * epsilon);
		posIte++;
		velIte++;
	}
	CHECK(true);
}

TEST_CASE("Control, Collision, Slopes", "[game]") {
	REQUIRE_NOTHROW(initializeLevel(testEnvironment()));
	std::pair<Player*, ControlModifierData*> pair;
	REQUIRE_NOTHROW(pair = initializePlayer());

	Player* player = pair.first;
	ControlModifierData* cmd = pair.second;

	player->setPos(vec3f(-4.0, 0.0, 0.0));
	player->vel = vec3f(0, 0, 0);
	cmd->horizontalVel = vec3f(0.0, 0.0, 0.05);
	arrivesAt(vec3f(0.0, 0.0, 4.5), player);

	player->setPos(vec3f(-4.0, 0.0, 0.0));
	player->vel = vec3f(0, 0, 0);
	cmd->horizontalVel = vec3f(0.0, 0.0, -0.1);
	arrivesAt(vec3f(0.0, 0.0, -4.5), player);

	player->setPos(vec3f(2.0, 0.0, 3.0));
	player->vel = vec3f(0, 0, 0);
	cmd->horizontalVel = vec3f(-0.1, 0.0, 0.02);
	arrivesAt(vec3f(-4.3, 0.0, 0.0), player);

	player->setPos(vec3f(-4.0, 0.0, 0.0));
	player->vel = vec3f(0, 0, 0);
	cmd->horizontalVel = vec3f(0.1, 0.0, 0.0);
	arrivesAt(vec3f(12.0, 3.0, 0.0), player);

	player->setPos(vec3f(-4.0, 0.0, 0.0));
	player->vel = vec3f(0, 0, 0);
	playerArrivesAt({ vec3f(0.0, 0.0, -4.0), vec3f(0.0,0.0,4.5) }, { vec3f(0.1,0.0,-0.1), vec3f(-0.01,0.0,0.15) }, player, cmd);


	REQUIRE_NOTHROW(terminateLevel());
}

TEST_CASE("Jump", "[game]") {
	REQUIRE_NOTHROW(initializeLevel(jumpEnvironment()));
	std::pair<Player*, ControlModifierData*> pair;
	REQUIRE_NOTHROW(pair = initializePlayer());

	Player* player = pair.first;
	ControlModifierData* cmd = pair.second;

	player->setPos(vec3f(-4.5, 0.0, 0.0));
	player->vel = vec3f(0, 0, 0);
	cmd->doJump = true;
	playerArrivesAt({ vec3f(4.5, 4.0, 0.0), vec3f(-4.5,0.0,0.0) }, { vec3f(0.1,0.0,0.0), vec3f(-0.1,0.0,0.0) }, player, cmd);


	REQUIRE_NOTHROW(terminateLevel());
}