#pragma once

#include <map>
#include <string>

struct ModifierData {
	bool markedRemove;
	ModifierData() : markedRemove(false) {}
};
class Modifiable;
class Modifier {
private:
	static std::map<std::string, Modifier*> modifiers;
	static std::map<const Modifier*, std::string> keys;

	bool serverOnly;
public:
	Modifier() : Modifier(true) {}
	Modifier(bool serverOnly) : serverOnly(serverOnly) {}
	bool isServerOnly() { return serverOnly; }
	static void registerModifier(std::string key, Modifier* modifier) {
		modifiers[key] = modifier;
		keys[modifier] = key;
	}
	static const Modifier* getModifier(std::string key) {
		return modifiers[key];
	}
	static std::string getKey(const Modifier* modifier) {
		return keys[modifier];
	}
	virtual void modify(Modifiable* obj, ModifierData* data) = 0;
};