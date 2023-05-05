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
	static void registerModifier(std::string key, Modifier* modifier) {
		modifiers[key] = modifier;
		keys[modifier] = key;
	}
	static Modifier* getModifier(std::string key) {
		return modifiers[key];
	}
	static std::string getKey(const Modifier* modifier) {
		return keys[modifier];
	}


	Modifier() : Modifier(true) {}
	Modifier(bool serverOnly) : serverOnly(serverOnly) {}
	bool isServerOnly() { return serverOnly; }

	virtual void modify(Modifiable* obj, ModifierData* data) = 0;
	virtual size_t size() = 0;
	virtual ModifierData* fromBytes(const uint8_t* data) = 0;
};