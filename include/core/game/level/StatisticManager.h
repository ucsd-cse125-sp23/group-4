#pragma once

#include <variant>
#include <map>
#include <string>
#include "core/game/event/EventManager.h";

using Stat = std::variant<int, float>;

struct StatisticDefinition {
	std::string key;
	Stat defaultValue;
	StatisticDefinition(std::string key, std::variant<int,float> defaultValue) : key(key), defaultValue(defaultValue) {}
	virtual void registerListeners(const EventManager* eventManager) = 0;
};
template<typename U>
struct LambdaStatisticDefinition : StatisticDefinition, private U {
	LambdaStatisticDefinition(std::string key, Stat defaultValue, U l) noexcept : StatisticDefinition(key, defaultValue), U{ l } {}

	void registerListeners(const EventManager* eventManager) {
		U::operator()();
	}
};
class StatisticManager {
private:
	const EventManager* eventManager;

	std::map<std::string, StatisticDefinition*> definitions;
	std::map<uint32_t, std::map<std::string, Stat>> playerStatistics;

	Stat getValue(uint32_t pid, StatisticDefinition* def);
public:
	StatisticManager(const EventManager* eventManager);

	void addDefinition(StatisticDefinition* def);

	template <typename T>
	Stat getValue(uint32_t pid, std::string key);
};