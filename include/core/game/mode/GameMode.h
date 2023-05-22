#pragma once

#include <vector>

#include "core/game/level/Level.h"

class GameMode {
 protected:
  Level* level;
  bool lowerScoreLead;

 public:
  GameMode(bool lowerScoreLead);
  virtual void registerTrackers(Level* l);
  virtual int queryScore(uint32_t pid) = 0;
  std::vector<std::vector<uint32_t>> queryPlacements();
  virtual void initPlayers(std::map<uint32_t, Player*> players) = 0;
};