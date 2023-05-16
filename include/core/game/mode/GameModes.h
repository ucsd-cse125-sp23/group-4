#pragma once

#include "core/game/mode/GameModeBases.h"

class OneTaggerCountGameMode : public NTaggerGameMode, public CountGameMode {
public:
	OneTaggerCountGameMode();
};
class OneTaggerTimeGameMode : public NTaggerGameMode, public TimeGameMode {
public:
	OneTaggerTimeGameMode();
};
class TeamedTaggersTimeGameMode : public TeamedTaggerGameMode, public CountGameMode {
public:
	TeamedTaggersTimeGameMode();
};
class ThreeTaggersTimeGameMode : public NTaggerGameMode, public CountGameMode {
public:
	ThreeTaggersTimeGameMode();
};