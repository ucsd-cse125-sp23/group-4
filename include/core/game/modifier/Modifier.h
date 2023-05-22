#pragma once

struct ModifierData {
  bool markedRemove;
  ModifierData() : markedRemove(false) {}
};
class Modifiable;
class Modifier {
 public:
  const bool serverOnly;
  Modifier();
  Modifier(bool serverOnly);
  virtual void modify(Modifiable* obj, ModifierData* data) = 0;
};