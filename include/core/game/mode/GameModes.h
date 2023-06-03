#pragma once

#include "core/game/mode/GameModeBases.h"

class OneTaggerCountGameMode : public NTaggerGameMode, public CountGameMode {
 public:
  OneTaggerCountGameMode();
  void registerTrackers(Level* level) override;
};
class OneTaggerTimeGameMode : public NTaggerGameMode, public TimeGameMode {
 public:
  OneTaggerTimeGameMode();
  void registerTrackers(Level* level) override;
};
class ThreeTaggersTimeGameMode : public NTaggerGameMode, public TimeGameMode {
 public:
  ThreeTaggersTimeGameMode();
  void registerTrackers(Level* level) override;
};
class TeamedTaggersTimeGameMode : public TeamedTaggerGameMode,
                                  public TimeGameMode {
 public:
  TeamedTaggersTimeGameMode();
  void registerTrackers(Level* level) override;
};
