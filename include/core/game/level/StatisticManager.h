#pragma once

#include <map>
#include <set>
#include <string>
#include <variant>

#include "core/game/event/EventManager.h"

using Stat = std::variant<std::monostate, int, float>;

class StatisticManager {
 private:
  std::set<std::string> keys;
  std::map<std::string, Stat> defaultValues;
  std::map<uint32_t, std::map<std::string, Stat>> playerStatistics;

 public:
  StatisticManager();

  bool registerStat(std::string key, Stat defaultValue);

  const std::set<std::string> getKeys();
  Stat getValue(uint32_t pid, std::string key);
  bool setValue(uint32_t pid, std::string key, Stat stat);
};
