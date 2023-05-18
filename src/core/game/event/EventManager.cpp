#include "core/game/event/EventManager.h"

#include "core/util/global.h"

EventManager::EventManager(Level* level) : level(level) {}

void EventManager::registerCollisionEventHandler(
    std::function<void(CollisionEvent)> handler) {
  this->collisionEventHandlers.push_back(handler);
}
void EventManager::onCollisionEvent(CollisionEvent event) {
  for (auto handler : this->collisionEventHandlers) handler(event);
}

void EventManager::registerTriggerEventHandler(
    std::function<void(TriggerEvent)> handler) {
  this->triggerEventHandlers.push_back(handler);
}
void EventManager::onTriggerEvent(TriggerEvent event) {
  for (auto handler : this->triggerEventHandlers) handler(event);
}

void EventManager::registerTaggingEventHandler(
    std::function<void(TaggingEvent)> handler) {
  this->taggingEventHandlers.push_back(handler);
}
void EventManager::onTaggingEvent(TaggingEvent event) {
  for (auto handler : this->taggingEventHandlers) handler(event);
}

void EventManager::fireCollisionEvent(PObject* self, PObject* other) {
  this->onCollisionEvent({level, self, other});
}
void EventManager::fireTriggerEvent(PObject* self, PObject* other) {
  this->onTriggerEvent({level, self, other});
}
void EventManager::fireTaggingEvent(PObject* tagger, PObject* tagee,
                                    int ticksIt) {
  this->onTaggingEvent({level, tagger, tagee, ticksIt});
}
