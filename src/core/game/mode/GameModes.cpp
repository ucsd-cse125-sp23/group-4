#include "core/game/mode/GameModes.h"


OneTaggerCountGameMode::OneTaggerCountGameMode() : NTaggerGameMode(1), GameMode(true)
{}
OneTaggerTimeGameMode::OneTaggerTimeGameMode() : NTaggerGameMode(1), GameMode(true)
{}
TeamedTaggersTimeGameMode::TeamedTaggersTimeGameMode() : TeamedTaggerGameMode(), GameMode(true)
{}
ThreeTaggersTimeGameMode::ThreeTaggersTimeGameMode() : NTaggerGameMode(3), GameMode(true)
{}