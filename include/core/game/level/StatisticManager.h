#pragma once

#include <any>
#include <map>
#include <string>
#include "core/game/event/EventManager.h";

struct Definition {
	std::string key;
};
template <typename T>
struct StatisticDefinition : Definition {
	T defaultValue;
	StatisticDefinition(std::string key, T defaultValue) : key(key), defaultValue(defaultValue) {}
	virtual void registerListeners(const EventManager* eventManager) = 0;
};
template<typename T, typename U>
struct LambdaStatisticDefinition : StatisticDefinition<T>, private U {
	LambdaStatisticDefinition(std::string key, T defaultValue, U l) noexcept : StatisticDefinition(key, defaultValue), U{ l } {}

	void registerListeners(const EventManager* eventManager) {
		U::operator()();
	}
};
class StatisticManager {
private:
	const EventManager* eventManager;

	std::map<std::string, Definition*> definitions;
	std::map<uint32_t, std::map<std::string, std::any>> playerStatistics;

	template <typename T>
	T getValue(uint32_t pid, StatisticDefinition<T>* def);
public:
	StatisticManager(const EventManager* eventManager);

	template <typename T>
	void addDefinition(StatisticDefinition<T>* def);

	template <typename T>
	T getValue(uint32_t pid, std::string key);
};