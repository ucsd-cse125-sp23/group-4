#include "core/game/level/StatisticManager.h"


template <typename T>
T StatisticManager::getValue(uint32_t pid, StatisticDefinition<T>* def)
{
	if (this->playerStatistics[pid].find(def->key) == this->playerStatistics[pid].end())
	{
		this->playerStatistics[pid].find[def->key] = def->defaultValue;
		return def->defaultValue;
	}
	return std::any_cast<decltype(def->defaultValue)>(this->playerStatistics[pid][def->key]);
}

StatisticManager::StatisticManager(const EventManager* eventManager) : eventManager(eventManager)
{}

template <typename T>
void StatisticManager::addDefinition(StatisticDefinition<T>* def)
{
	this->definitions[def->key] = def;
	def->registerListeners(evetnManager);
}

template <typename T>
T getValue(uint32_t pid, std::string key)
{
	return getValue(pid, this->definitions[key])
}