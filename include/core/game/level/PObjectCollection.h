#pragma once

#include <map>

#include "core/game/physics/PObject.h"

class PObjectCollection {
 private:
  std::map<size_t, PObject*> objs;

 public:
  PObjectCollection();
  void addPObject(PObject* obj);
  PObject* getById(size_t id);
  bool removeById(size_t id);
  std::vector<size_t> getAllIds();
  void clear();

  PObject* operator[](size_t id);
};