#pragma once

enum class Event {
  ReducedGravity,
  IncreasedGravity,
  SlipperyPhysics,  // ReducedFriction?
  Speed,            // IncreasedVelocity?
  Slow,             // ReducedVelocity?
  Freeze,           // idrk what this means
  Confuse,          // same ^
  Attract,
  Repel
};
