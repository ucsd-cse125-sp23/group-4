#include "core/game/event/EventManager.h"

#include "core/util/global.h"

EventManager::EventManager(Level* level) : level(level) {}

void EventManager::onCollisionEvent(CollisionEvent event) {
  for (auto handler : this->collisionEventHandlers) handler(event);
}
void EventManager::onTriggerEvent(TriggerEvent event) {
  for (auto handler : this->triggerEventHandlers) handler(event);
}
void EventManager::onTaggingEvent(TaggingEvent event) {
  for (auto handler : this->taggingEventHandlers) handler(event);
}
void EventManager::onJumpEvent(JumpEvent event) {
  for (auto handler : this->jumpEventHandlers) handler(event);
}
void EventManager::onLandEvent(LandEvent event) {
  for (auto handler : this->landEventHandlers) handler(event);
}
void EventManager::onPickupEvent(PickupEvent event) {
  for (auto handler : this->pickupEventHandlers) handler(event);
}

void EventManager::registerEventHandler(
    std::function<void(CollisionEvent)> handler) {
  this->collisionEventHandlers.push_back(handler);
}
void EventManager::registerEventHandler(
    std::function<void(TriggerEvent)> handler) {
  this->triggerEventHandlers.push_back(handler);
}
void EventManager::registerEventHandler(
    std::function<void(TaggingEvent)> handler) {
  this->taggingEventHandlers.push_back(handler);
}
void EventManager::registerEventHandler(
    std::function<void(JumpEvent)> handler) {
  this->jumpEventHandlers.push_back(handler);
}
void EventManager::registerEventHandler(
    std::function<void(LandEvent)> handler) {
  this->landEventHandlers.push_back(handler);
}
void EventManager::registerEventHandler(
    std::function<void(PickupEvent)> handler) {
  this->pickupEventHandlers.push_back(handler);
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
void EventManager::fireJumpEvent(PObject* self) {
  this->onJumpEvent({level, self});
}
void EventManager::fireLandEvent(PObject* self) {
  this->onLandEvent({level, self});
}
void EventManager::firePickupEvent(PObject* self) {
  this->onPickupEvent({level, self});
}
