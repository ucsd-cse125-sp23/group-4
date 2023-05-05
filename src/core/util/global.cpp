#include "core/util/global.h"


int PLAYER_LAYER = 0;
int ENVIRONMENT_LAYER = 1;
int POWER_LAYER = 2;

CylinderShape* PLAYER_BASE_SHAPE = new CylinderShape(1.0, 0.5);
OffsetShape* PLAYER_BOUNDING_SHAPE = new OffsetShape(PLAYER_BASE_SHAPE, vec3f(0.0f, 0.5f, 0.0f));

Level* level = nullptr;