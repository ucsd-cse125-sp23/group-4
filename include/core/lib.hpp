#pragma once

#include <utility>

#include "core/game/mode/GameMode.h"
#include "core/game/mode/Gamemodes.h"
#include "core/game/modifier/ControlModifier.h"
#include "core/game/physics/Player.h"
#include "core/util/global.h"

void initializeLevel(Environment* environment);
void terminateLevel();
std::pair<Player*, ControlModifierData*> initializePlayer();

void applyGameMode(GameMode* gamemode);
int queryScore(uint32_t pid);
std::vector<std::vector<uint32_t>> queryPlacements();
void initPlayers(std::map<uint32_t, Player*> players);
