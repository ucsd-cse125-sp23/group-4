#include "ItemBox.h"

#include <cmath>
#include <glm/glm.hpp>

using glm::mat4x4;
using glm::vec3;
using glm::vec4;

void ItemBox::update(float dt) {
  if (fx) fx->update(dt);

  GameThing::update(dt);
}
