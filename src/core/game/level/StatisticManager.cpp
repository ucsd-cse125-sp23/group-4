#include "core/game/level/StatisticManager.h"

StatisticManager::StatisticManager() {}

bool StatisticManager::registerStat(std::string key, Stat defaultValue) {
  if (defaultValues.find(key) == defaultValues.end()) {
    defaultValues[key] = defaultValue;
    keys.insert(key);
    return true;
  }
  return false;
}

const std::set<std::string> StatisticManager::getKeys() { return keys; }

Stat StatisticManager::getValue(uint32_t pid, std::string key) {
  if (defaultValues.find(key) == defaultValues.end()) return std::monostate();
  if (playerStatistics[pid].find(key) == playerStatistics[pid].end())
    playerStatistics[pid][key] = defaultValues[key];
  return playerStatistics[pid][key];
}
bool StatisticManager::setValue(uint32_t pid, std::string key, Stat stat) {
  if (defaultValues.find(key) == defaultValues.end()) return false;
  playerStatistics[pid][key] = stat;
}
