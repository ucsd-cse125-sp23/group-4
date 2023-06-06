#pragma once

#include <cstdint>
class Level;
struct ModifierData {
  Level* level;
  bool markedRemove;
  std::uint64_t expire = -1;
  /*
   * @param duration duration of the modifier instance before expiring (0
   * duration modifiers will never expire)
   *
   */
  explicit ModifierData(Level* level, std::uint64_t duration = 0);
  void markExpired();
};
class Modifiable;
class Modifier {
 public:
  Modifier();
  virtual void modify(Modifiable* obj, ModifierData* data) = 0;
};
