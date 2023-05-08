#pragma once

#include "core/util/global.h"
#include "core/game/physics/Player.h"
#include "core/game/modifier/ControlModifier.h"


void initializeLib(bool isServer);
void initializeLevel(Environment* environment);
void terminateLevel();
void terminateLib();
std::pair<Player*,ControlModifierData*> initializePlayer();