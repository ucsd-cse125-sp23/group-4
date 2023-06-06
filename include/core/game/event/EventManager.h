#pragma once

#include <functional>
#include <vector>

#include "Event.h"

class EventManager {
 private:
  Level* level;

  std::vector<std::function<void(CollisionEvent)>> collisionEventHandlers;
  std::vector<std::function<void(TriggerEvent)>> triggerEventHandlers;
  std::vector<std::function<void(TaggingEvent)>> taggingEventHandlers;
  std::vector<std::function<void(JumpEvent)>> jumpEventHandlers;
  std::vector<std::function<void(LandEvent)>> landEventHandlers;
  std::vector<std::function<void(PickupEvent)>> pickupEventHandlers;
  void onCollisionEvent(CollisionEvent event);
  void onTriggerEvent(TriggerEvent event);
  void onTaggingEvent(TaggingEvent event);
  void onJumpEvent(JumpEvent event);
  void onLandEvent(LandEvent event);
  void onPickupEvent(PickupEvent event);

 public:
  explicit EventManager(Level* level);
  void registerEventHandler(std::function<void(CollisionEvent)> handler);
  void registerEventHandler(std::function<void(TriggerEvent)> handler);
  void registerEventHandler(std::function<void(TaggingEvent)> handler);
  void registerEventHandler(std::function<void(JumpEvent)> handler);
  void registerEventHandler(std::function<void(LandEvent)> handler);
  void registerEventHandler(std::function<void(PickupEvent)> handler);

  void fireCollisionEvent(PObject* self, PObject* other);
  void fireTriggerEvent(PObject* self, PObject* other);
  void fireTaggingEvent(PObject* tagger, PObject* tagee, int ticksIt);
  void fireJumpEvent(PObject* self);
  void fireLandEvent(PObject* self);
  void firePickupEvent(PObject* self);
};
