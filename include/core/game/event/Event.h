#pragma once

#include "core/game/physics/PObject.h"

class Event {
 public:
  Level* level;
  explicit Event(Level* level) : level(level) {}
};
class CollisionEvent : public Event {
 public:
  PObject *self, *other;
  CollisionEvent(Level* level, PObject* self, PObject* other)
      : Event(level), self(self), other(other) {}
};
class TriggerEvent : public Event {
 public:
  PObject *self, *other;
  TriggerEvent(Level* level, PObject* self, PObject* other)
      : Event(level), self(self), other(other) {}
};
class TaggingEvent : public Event {
 public:
  PObject *tagger, *tagee;
  int ticksIt;
  TaggingEvent(Level* level, PObject* tagger, PObject* tagee, int ticksIt)
      : Event(level), tagger(tagger), tagee(tagee), ticksIt(ticksIt) {}
};
class JumpEvent : public Event {
 public:
  PObject* self;
  JumpEvent(Level* level, PObject* self)
      : Event(level), self(self) {}
};
class LandEvent : public Event {
 public:
  PObject* self;
  LandEvent(Level* level, PObject* self) : Event(level), self(self) {}
};
class PickupEvent : public Event {
 public:
  PObject* self;
  PickupEvent(Level* level, PObject* self) : Event(level), self(self) {}
};