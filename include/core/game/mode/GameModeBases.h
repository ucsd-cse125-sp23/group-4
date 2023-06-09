#pragma once

#include <map>
#include <vector>

#include "core/game/mode/GameMode.h"

class CountGameMode : virtual public GameMode {
 public:
  CountGameMode();
  void registerTrackers(Level* level) override;
  int queryScore(uint32_t pid) override;
};
class TimeGameMode : virtual public GameMode {
 public:
  TimeGameMode();
  void registerTrackers(Level* level) override;
  int queryScore(uint32_t pid) override;
};
class NTaggerGameMode : virtual public GameMode {
 private:
  int n;

 public:
  explicit NTaggerGameMode(int n);
  void initPlayers(std::map<uint32_t, core::Player*> players) override;
};
class TeamedTaggerGameMode : virtual public GameMode {
 public:
  std::vector<uint32_t> teams[2];
  TeamedTaggerGameMode();
  void initPlayers(std::map<uint32_t, core::Player*> players) override;
};
