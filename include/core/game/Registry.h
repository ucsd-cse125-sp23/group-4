#pragma once

#include <map>
#include <string>

#include "core/game/modifier/Modifier.h"
#include "core/game/physics/PObjectType.h"

template <typename T>
class Registry {
private:
	std::string key;
	std::map<std::string, T*> forwardMap;
	std::map<T*, std::string> reverseMap;
public:
	Registry(std::string key) : key(key) {}
	void registerType(std::string key, T* t) {
		forwardMap[key] = t;
		reverseMap[t] = key;
	}
	std::string getKey(T* t) {
		return reverseMap[t];
	}
	T* getType(std::string key) {
		return forwardMap[key];
	}
};

class GameRegistry {
public:
	Registry<Modifier> MODIFIER_REGISTRY = Registry<Modifier>("modifier");
	Registry<PObjectType> POBJECT_REGISTRY = Registry<PObjectType>("pobject");
};