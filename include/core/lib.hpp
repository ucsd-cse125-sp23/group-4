#pragma once

#include "core/util/global.h"
#include "core/game/physics/Player.h"
#include "core/game/modifier/ControlModifier.h"


void initializeLib();
void initializeLevel(Environment* environment);
void terminateLevel();
std::pair<Player*,ControlModifierData*> initializePlayer();