#pragma once

#include "core/game/physics/PObject.h"

#include <map>

class PObjectCollection {
private:
	std::map<size_t, PObject*> objs;
public:
	PObjectCollection();
	void addPObject(PObject* obj);
	PObject* getById(size_t id);
	bool removeById(size_t id);
	std::vector<size_t> getAllIds();

	PObject* operator[](size_t id);
};