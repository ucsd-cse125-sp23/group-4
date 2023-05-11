#include "core/game/level/StatisticManager.h"


Stat StatisticManager::getValue(uint32_t pid, StatisticDefinition* def)
{
	if (this->playerStatistics[pid].find(def->key) == this->playerStatistics[pid].end())
	{
		this->playerStatistics[pid][def->key] = def->defaultValue;
		return def->defaultValue;
	}
	return this->playerStatistics[pid][def->key];
}

StatisticManager::StatisticManager(const EventManager* eventManager) : eventManager(eventManager)
{}

void StatisticManager::addDefinition(StatisticDefinition* def)
{
	this->definitions[def->key] = def;
	def->registerListeners(eventManager);
}

template <typename T>
T getValue(uint32_t pid, std::string key)
{
	return getValue(pid, this->definitions[key])
}