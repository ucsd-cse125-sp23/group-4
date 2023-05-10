#include "core/game/level/PObjectCollection.h"

PObjectCollection::PObjectCollection()
{
}
void PObjectCollection::addPObject(PObject* obj)
{
	this->objs[obj->id] = obj;
}
PObject* PObjectCollection::getById(size_t id)
{
	return this->objs.find(id) == this->objs.end() ? nullptr : this->objs[id];
}
bool PObjectCollection::removeById(size_t id)
{
	auto ite = this->objs.find(id);
	if (ite != this->objs.end())
	{
		this->objs.erase(ite);
		return true;
	}
	return false;
}
std::vector<size_t> PObjectCollection::getAllIds()
{
	std::vector<size_t> keys;
	for (auto pair : this->objs)
		keys.push_back(pair.first);
	return keys;
}
void PObjectCollection::clear()
{
	this->objs.clear();
}

PObject* PObjectCollection::operator[](size_t id)
{
	return this->getById(id);
}