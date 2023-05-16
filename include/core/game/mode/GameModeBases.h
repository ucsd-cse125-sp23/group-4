#pragma once

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
	NTaggerGameMode(int n);
	void initPlayers(std::map<uint32_t, Player*> players) override;
};
class TeamedTaggerGameMode : virtual public GameMode {
public:
	TeamedTaggerGameMode();
	void initPlayers(std::map<uint32_t, Player*> players) override;
};