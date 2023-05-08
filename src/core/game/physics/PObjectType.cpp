#include "core/game/physics/PObjectType.h"


PObjectType::PObjectType(std::string key, std::function<PObject* ()> func) : key(key), instansiate(func)
{}