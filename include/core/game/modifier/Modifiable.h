#pragma once

#include <algorithm>
#include <map>
#include <vector>
#include <algorithm>

#include "core/game/modifier/ModifierInstance.h"

class Modifiable {
 private:
  std::map<Modifier*, std::vector<ModifierInstance*>> modifiers;

 public:
  virtual ~Modifiable() {}
  void addModifierInstance(ModifierInstance* instance) {
    if (modifiers.count(instance->getModifier()) == 0)
      modifiers[instance->getModifier()] = std::vector<ModifierInstance*>();
    modifiers[instance->getModifier()].push_back(instance);
  }
  bool hasModifier(Modifier* modifier) {
    return modifiers.count(modifier) != 0 && modifiers[modifier].size() > 0;
  }
  bool removeModifierInstance(ModifierInstance* instance) {
    if (modifiers.count(instance->getModifier()) != 0) {
      auto list = modifiers[instance->getModifier()];
      auto ite = find(list.begin(), list.end(), instance);
      if (ite != list.end()) {
        delete (*ite);
        list.erase(ite);
        return true;
      }
    }
    return false;
  }
  bool removeModifier(Modifier* modifier) {
    if (modifiers.count(modifier) != 0) {
      for (ModifierInstance* instance : modifiers[modifier]) delete instance;
      modifiers[modifier].clear();
      return true;
    }
    return false;
  }
  void removeMarkedModifier() {
    for (auto pair : modifiers) {
      for (int i = 0; i < pair.second.size(); i++) {
        if (static_cast<ModifierData*>(pair.second[i]->get())->markedRemove) {
          delete pair.second[i];
          pair.second[i] = pair.second.back();
          pair.second.pop_back();
          i--;
        }
      }
    }
  }
  const std::map<Modifier*, std::vector<ModifierInstance*>> getModifiers() {
    return modifiers;
  }
  const std::vector<ModifierInstance*> getModifiers(Modifier* modifier) {
    return modifiers[modifier];
  }
  void tick() {
    for (auto pair : this->getModifiers()) {
      for (auto instance : pair.second)
        pair.first->modify(this, instance->get());
    }
    removeMarkedModifier();
  }
};
