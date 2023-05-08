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
	bool serverOnly;
public:
	Modifier() : Modifier(true) {}
	Modifier(bool serverOnly) : serverOnly(serverOnly) {}
	bool isServerOnly() { return serverOnly; }

	virtual void modify(Modifiable* obj, ModifierData* data) = 0;
	virtual size_t size() = 0;
	virtual ModifierData* fromBytes(const uint8_t* data) = 0;
};