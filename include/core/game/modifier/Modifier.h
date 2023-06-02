#pragma once

struct ModifierData {
  bool markedRemove;
  unsigned long long expire = -1;
  /*
   * @param duration duration of the modifier instance before expiring (0
   * duration modifiers will never expire)
   *
   */
  ModifierData(unsigned long long duration = 0);
  void markExpired();
};
class Modifiable;
class Modifier {
 public:
  Modifier();
  virtual void modify(Modifiable* obj, ModifierData* data) = 0;
};