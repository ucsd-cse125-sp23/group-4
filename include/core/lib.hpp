#pragma once

#include <map>
#include <utility>
#include <vector>

#include "core/game/mode/GameMode.h"
#include "core/game/modifier/ControlModifier.h"
#include "core/game/physics/Player.h"
#include "core/game/physics/PowerUp.h"
#include "core/game/physics/Mushroom.h"
#include "core/game/physics/River.h"
#include "core/util/global.h"

Level* initializeLevel(Environment* environment);
std::pair<core::Player*, ControlModifierData*> initializePlayer(Level* level);

void applyGameMode(Level* level, GameMode* gamemode);
int queryScore(Level* level, uint32_t pid);
std::vector<std::vector<uint32_t>> queryPlacements(Level* level);
void initPlayers(Level* level, std::map<uint32_t, core::Player*> players);

std::vector<PowerUp*> getPowerUps(Level* level);
bool isMoving(core::Player* player);
