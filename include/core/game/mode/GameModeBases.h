#pragma once

#include "core/game/mode/GameMode.h"

class CountGameMode : virtual public GameMode {
public:
	CountGameMode();
	virtual void registerTrackers(Level* level) override;
	virtual int queryScore(uint32_t pid) override;
};
class TimeGameMode : virtual public GameMode {
public:
	TimeGameMode();
	virtual void registerTrackers(Level* level) override;
	virtual int queryScore(uint32_t pid) override;
};
class NTaggerGameMode : virtual public GameMode {
private:
	int n;
public:
	NTaggerGameMode(int n);
	virtual void initPlayers(std::map<uint32_t, Player*> players) override;
};
class TeamedTaggerGameMode : virtual public GameMode {
public:
	std::vector<uint32_t> teams[2];
	TeamedTaggerGameMode();
	virtual void initPlayers(std::map<uint32_t, Player*> players) override;
};