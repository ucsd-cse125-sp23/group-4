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
  void onCollisionEvent(CollisionEvent event);
  void onTriggerEvent(TriggerEvent event);
  void onTaggingEvent(TaggingEvent event);

 public:
  explicit EventManager(Level* level);
  void registerCollisionEventHandler(
      std::function<void(CollisionEvent)> handler);
  void registerTriggerEventHandler(std::function<void(TriggerEvent)> handler);
  void registerTaggingEventHandler(std::function<void(TaggingEvent)> handler);

  void fireCollisionEvent(PObject* self, PObject* other);
  void fireTriggerEvent(PObject* self, PObject* other);
  void fireTaggingEvent(PObject* tagger, PObject* tagee, int ticksIt);
};
