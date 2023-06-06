#pragma once

#include <map>
#include <utility>
#include <vector>

#include "core/game/mode/GameMode.h"
#include "core/game/mode/GameModes.h"
#include "core/game/modifier/ControlModifier.h"
#include "core/game/physics/Player.h"
#include "core/util/global.h"

Level* initializeLevel(Environment* environment);
void terminateLevel();
std::pair<Player*, ControlModifierData*> initializePlayer();

void applyGameMode(GameMode* gamemode);
int queryScore(uint32_t pid);
std::vector<std::vector<uint32_t>> queryPlacements();
void initPlayers(std::map<uint32_t, Player*> players);
