#pragma once

#include <map>
#include <vector>

#include "core/game/physics/Player.h"

class Level;
class GameMode {
 protected:
  Level* level;
  bool lowerScoreLead;

 public:
  explicit GameMode(bool lowerScoreLead);
  virtual void registerTrackers(Level* l);
  virtual int queryScore(uint32_t pid) = 0;
  std::vector<std::vector<uint32_t>> queryPlacements();
  virtual void initPlayers(std::map<uint32_t, core::Player*> players) = 0;
};
