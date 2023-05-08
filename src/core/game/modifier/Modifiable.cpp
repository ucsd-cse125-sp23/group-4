#include "core/game/modifier/Modifiable.h"

#include "core/util/global.h"


Modifiable::~Modifiable() {
}
void Modifiable::addModifierInstance(ModifierInstance* instance) {
	if (modifiers.count(instance->getModifier()) == 0)
		modifiers[instance->getModifier()] = std::vector<ModifierInstance*>();
	modifiers[instance->getModifier()].push_back(instance);
}
bool Modifiable::hasModifier(Modifier* modifier) {
	return modifiers.count(modifier) != 0 && modifiers[modifier].size() > 0;
}
bool Modifiable::removeModifierInstance(ModifierInstance* instance) {
	if (modifiers.count(instance->getModifier()) != 0)
	{
		auto list = modifiers[instance->getModifier()];
		auto ite = find(list.begin(), list.end(), instance);
		if (ite != list.end())
		{
			delete (*ite);
			list.erase(ite);
			return true;
		}
	}
	return false;
}
bool Modifiable::removeModifier(Modifier* modifier) {
	if (modifiers.count(modifier) != 0)
	{
		for (ModifierInstance* instance : modifiers[modifier])
			delete instance;
		modifiers[modifier].clear();
		return true;
	}
	return false;
}
void Modifiable::removeMarkedModifier() {
	for (auto pair : modifiers) {
		for (int i = 0; i < pair.second.size(); i++)
		{
			if (static_cast<ModifierData*>(pair.second[i]->get())->markedRemove)
			{
				delete pair.second[i];
				pair.second[i] = pair.second.back();
				pair.second.pop_back();
				i--;
			}
		}
	}
}
const std::map<Modifier*, std::vector<ModifierInstance*>> Modifiable::getModifiers() {
	return modifiers;
}
void Modifiable::tick() {
	for (auto pair : this->getModifiers()) {
		for (auto instance : pair.second)
			pair.first->modify(this, instance->get());
	}
	removeMarkedModifier();
}

void Modifiable::pack(ByteBufferBuilder& builder) {
	size_t count = 0;
	ByteBufferBuilder modBuilder;
	for (auto pair : modifiers)
	{
		if (!pair.first->isServerOnly())
		{
			count++;
			std::string key = GAME_REGISTRY->MODIFIER_REGISTRY.getKey(pair.first);
			modBuilder.writeUInt(key.length());
			modBuilder.writeString(key);
			modBuilder.writeULL(pair.second.size());
			for (ModifierInstance* inst : pair.second)
			{
				ByteBuffer buf = ByteBuffer((uint8_t*)inst->get(), pair.first->size());
				modBuilder.writeBuffer(buf);
			}
		}
	}
	builder.writeULL(count);
	builder.writeBuffer(modBuilder.build());
}
void Modifiable::unpack(ByteBuffer buf) {
	size_t modCount = buf.nextULL();
	for (size_t i = 0; i < modCount; i++)
	{
		uint32_t strLen = buf.nextUInt();
		std::string key = buf.nextString(strLen);
		Modifier* modifier = GAME_REGISTRY->MODIFIER_REGISTRY.getType(key);

		for (auto inst : modifiers[modifier])
			delete inst;
		modifiers[modifier].clear();

		size_t instCount = buf.nextULL();
		for (size_t j = 0; j < instCount; j++)
		{
			ByteBuffer inst = buf.nextBuffer(modifier->size());
			modifiers[modifier].push_back(new ModifierInstance(modifier, modifier->fromBytes(inst.getBytes())));
		}
	}
}