#pragma once

struct ModifierData {
	bool markedRemove;
	ModifierData() : markedRemove(false) {}
};
class Modifiable;
class Modifier {
public:
	virtual void modify(Modifiable* obj, ModifierData* data) = 0;
};