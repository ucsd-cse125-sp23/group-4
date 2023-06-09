#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

#include "core/game/modifier/ModifierInstance.h"

class Modifiable {
 private:
  std::map<Modifier*, std::map<size_t,ModifierInstance*>> modifiers;

 public:
  virtual ~Modifiable() {}
  void addModifierInstance(ModifierInstance* instance) {
    if (modifiers.count(instance->getModifier()) == 0)
      modifiers[instance->getModifier()] = std::map<size_t,ModifierInstance*>();
    modifiers[instance->getModifier()][instance->id()] = instance;
  }
  bool hasModifier(Modifier* modifier) {
    return modifiers.count(modifier) != 0 && modifiers[modifier].size() > 0;
  }
  bool removeModifierInstance(ModifierInstance* instance) {
    if (modifiers.count(instance->getModifier()) != 0) {
      auto map = modifiers[instance->getModifier()];
      auto ite = map.find(instance->id());
      if (ite != map.end()) {
        delete (*ite).second;
        map.erase(ite);
        return true;
      }
    }
    return false;
  }
  bool removeModifier(Modifier* modifier) {
    if (modifiers.count(modifier) != 0) {
      for (auto pair : modifiers[modifier]) delete pair.second;
      modifiers[modifier].clear();
      return true;
    }
    return false;
  }
  void removeMarkedModifier() {
    for (auto& pair : modifiers) {
      for (auto pair2 : pair.second) {
        if (pair2.second->get()->markedRemove) {
          delete pair2.second;
          pair.second.erase(pair2.first);
        }
      }
    }
  }
  const std::map<Modifier*, std::map<size_t,ModifierInstance*>> getModifiers() {
    return modifiers;
  }
  const std::vector<ModifierInstance*> getModifiers(Modifier* modifier) {
    std::vector<ModifierInstance*> ret;
    for (auto pair : modifiers[modifier]) ret.push_back(pair.second);
    return ret;
  }
  void tick() {
    for (auto pair : modifiers) {
      for (auto pair2 : pair.second) {
        pair.first->modify(this, pair2.second->get());
        pair2.second->get()->markExpired();
      }
    }
    removeMarkedModifier();
  }
};
