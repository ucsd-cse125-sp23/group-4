#include "core/game/physics/River.h"
#include "core/math/shape/ConvexMeshShape.h"
#include "core/util/global.h"

River::River(std::vector<vec3f> vertices)
    : PObject(new ConvexMeshShape(vertices), ENVIRONMENTAL_EFFECT_LAYER) {}

River::~River() {}

void River::onTrigger(PObject* other) {
  other->addModifierInstance(new ModifierInstance(SPEEDBOOST_MODIFIER, new SpeedBoostModifierData(other->level, 5, -0.2f), 4));
}