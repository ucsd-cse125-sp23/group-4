#pragma once

#include <cstdint>
struct ModifierData {
  bool markedRemove;
  std::uint64_t expire = -1;
  /*
   * @param duration duration of the modifier instance before expiring (0
   * duration modifiers will never expire)
   *
   */
  explicit ModifierData(std::uint64_t duration = 0);
  void markExpired();
};
class Modifiable;
class Modifier {
 public:
  Modifier();
  virtual void modify(Modifiable* obj, ModifierData* data) = 0;
};
