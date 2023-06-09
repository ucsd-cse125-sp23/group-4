#include "core/game/physics/Mushroom.h"
#include "core/math/shape/ConvexMeshShape.h"
#include "core/util/global.h"

Mushroom::Mushroom(std::vector<vec3f> vertices)
    : PObject(new ConvexMeshShape(vertices), ENVIRONMENTAL_EFFECT_LAYER) {}

Mushroom::~Mushroom() {
}

void Mushroom::onTrigger(PObject* other) {
  other->addModifierInstance(new ModifierInstance(
      LAUNCH_MODIFIER, new LaunchModifierData(other->level, 1.2f, this->getPos()),
      4));
}