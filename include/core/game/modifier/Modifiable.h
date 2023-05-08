#pragma once

#include <map>
#include <vector>
#include <algorithm>

#include "core/game/Serializable.h"
#include "core/util/ByteBuffer.h"
#include "core/game/modifier/ModifierInstance.h"

class Modifiable : public Serializable {
private:
	std::map<Modifier*,std::vector<ModifierInstance*>> modifiers;
public:
	virtual ~Modifiable();
	void addModifierInstance(ModifierInstance* instance);
	bool hasModifier(Modifier* modifier);
	bool removeModifierInstance(ModifierInstance* instance);
	bool removeModifier(Modifier* modifier);
	void removeMarkedModifier();
	const std::map<Modifier*, std::vector<ModifierInstance*>> getModifiers();
	void tick();

	virtual void pack(ByteBufferBuilder& builder);
	virtual void unpack(ByteBuffer buf);
};