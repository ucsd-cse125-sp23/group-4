#pragma once

#include <any>

#include "core/game/modifier/Modifier.h"

class ModifierInstance {
 private:
  Modifier* modifier;
  ModifierData* data;
  size_t id_;

 public:
  ModifierInstance(Modifier* modifier, ModifierData* data, size_t id)
      : modifier(modifier), data(data), id_(id) {}
  ~ModifierInstance() { delete data; }
  Modifier* getModifier() { return modifier; }
  ModifierData* get() { return data; }
  size_t id() { return id_; }
};
