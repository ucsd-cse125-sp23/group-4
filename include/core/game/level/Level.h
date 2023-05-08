#pragma once

#include <vector>

#include "core/game/physics/PObject.h"
#include "core/game/level/Environment.h"
#include "core/game/level/PObjectCollection.h"

enum class CollisionType {
	NONE, COLLISION, TRIGGER
};
class Level : public Serializable {
private:
	unsigned long long age;
	CollisionType collisionTypeLUT[10][10];
	PObjectCollection objects;
	Environment* environment;
public:
	/**
	 * @param environment defines the environement of the level. Will be deleted on level deconstruction
	 */
	Level(Environment* environment) : environment(environment), age(0) {
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				collisionTypeLUT[i][j] = CollisionType::NONE;
	}
	~Level() {
		delete environment;
	}
	void setCollisionType(CollisionType type, int layer0, int layer1) {
		collisionTypeLUT[layer0][layer1] = type;
		collisionTypeLUT[layer1][layer0] = type;
	}
	void addPObject(PObject* obj) {
		objects.addPObject(obj);
	}
	void removePObject(size_t id) {
		objects.removeById(id);
	}
	unsigned long long getAge() { return age; }

	void tick();

	void pack(ByteBufferBuilder& builder);
	void unpack(ByteBuffer buf);
};