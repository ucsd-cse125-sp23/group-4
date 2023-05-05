#pragma once

#include "Event.h"

#include <vector>

class EventManager {
private:
	std::vector<CollisionEventHandler*> collisionEventHandlers;
	std::vector<TriggerEventHandler*> triggerEventHandlers;
	void onCollisionEvent(CollisionEvent event);
	void onTriggerEvent(TriggerEvent event);
public:
	void registerCollisionEventHandler(CollisionEventHandler* handler);
	void registerTriggerEventHandler(TriggerEventHandler* handler);

	void fireCollisionEvent(PObject* self, PObject* other);
	void fireTriggerEvent(PObject* self, PObject* other);
};