#pragma once

#include "core/game/modifier/ControlModifier.h"
#include "core/game/physics/Player.h"
#include "core/util/global.h"

void initializeLevel(Environment* environment);
void terminateLevel();
std::pair<Player*, ControlModifierData*> initializePlayer();
void spawnPowerUp(vec3f min, vec3f max, const std::vector<GlobalEffect*>& effects);