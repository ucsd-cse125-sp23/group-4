#pragma once

#include <vector>

#include "core/math/vector.h"
#include "core/game/modifier/Modifiable.h"
#include "core/game/modifier/ModifierInstance.h"
#include "core/game/physics/CollisionBounds.h"

class Level;
class PObject : public Modifiable {
private:
	static uint32_t maxId;
	bool toRemove = false;
	vec3f pos;

	CollisionBounds* bounds;
public:
	uint32_t id;
	vec3f vel, oPos;
	bool onGround, static_;
	PObject(BoundingShape* shape, unsigned int layer = 0, float friction = 0.0f, bool static_ = false);
	~PObject();
	vec3f getPos();
	void setPos(vec3f pos);
	void addPos(vec3f dPos);
	bool markedRemove();
	void markRemove();
	bool isStatic();
	const CollisionBounds* getBounds();
	virtual void tick();
	virtual void onCollision(PObject* other) {}
	virtual void onTrigger(PObject* other) {}
};