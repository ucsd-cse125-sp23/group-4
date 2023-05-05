#pragma once

#include <vector>

#include "core/game/physics/PObject.h"
#include "core/game/level/Environment.h"
#include "core/game/event/EventManager.h"
#include "core/game/level/StatisticManager.h"


enum class CollisionType {
	NONE, COLLISION, TRIGGER
};
class Level {
private:
	unsigned long long age;
	CollisionType collisionTypeLUT[10][10];
	std::vector<PObject*> objects;
	Environment* environment;

	EventManager* eventManager;
	StatisticManager* statisticManager;
public:
	/**
	 * @param environment defines the environement of the level. Will be deleted on level deconstruction
	 */
	Level(Environment* environment) : environment(environment), age(0), eventManager(new EventManager()) {
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				collisionTypeLUT[i][j] = CollisionType::NONE;
		statisticManager = new StatisticManager(eventManager);
	}
	~Level() {
		delete environment;
	}
	void setCollisionType(CollisionType type, int layer0, int layer1) {
		collisionTypeLUT[layer0][layer1] = type;
		collisionTypeLUT[layer1][layer0] = type;
	}
	void addPObject(PObject* obj) {
		objects.push_back(obj);
	}
	template <typename T>
	void addStatistic(StatisticDefinition<T>* def) {
		statisticManager->addDefinition(def);
	}
	unsigned long long getAge() { return age; }

	void tick();
};