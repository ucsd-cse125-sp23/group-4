#include "core/game/event/EventManager.h"

#include "core/util/global.h"

void EventManager::registerCollisionEventHandler(CollisionEventHandler* handler)
{
	this->collisionEventHandlers.push_back(handler);
}
void EventManager::onCollisionEvent(CollisionEvent event)
{
	for (auto handler : this->collisionEventHandlers)
		handler->onCollision(event);
}

void EventManager::registerTriggerEventHandler(TriggerEventHandler* handler)
{
	this->triggerEventHandlers.push_back(handler);
}
void EventManager::onTriggerEvent(TriggerEvent event)
{
	for (auto handler : this->triggerEventHandlers)
		handler->onTrigger(event);
}


void EventManager::fireCollisionEvent(PObject* self, PObject* other)
{
	this->onCollisionEvent({ self, other });
}
void EventManager::fireTriggerEvent(PObject* self, PObject* other)
{
	this->onTriggerEvent({ self, other });
}