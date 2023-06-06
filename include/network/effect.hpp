#pragma once

enum class Effect {
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
