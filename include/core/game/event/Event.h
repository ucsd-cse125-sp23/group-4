#pragma once

#include "core/game/physics/PObject.h"

struct CollisionEvent {
	PObject *self, *other;
};
class CollisionEventHandler {
public:
	virtual void onCollision(CollisionEvent event) = 0;
};

struct TriggerEvent {
	PObject *self, *other;
};
class TriggerEventHandler {
public:
	virtual void onTrigger(TriggerEvent event) = 0;
};