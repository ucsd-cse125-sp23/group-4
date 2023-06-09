#include "core/game/physics/Player.h"

#include "core/util/global.h"

using namespace core;  // NOLINT

uint32_t core::Player::maxId = 1;

core::Player::Player()
    : PObject(PLAYER_BOUNDING_SHAPE, PLAYER_LAYER, 0, false),
      look(vec3f(1.0f, 0.0f, 0.0f)) {
  this->pid = Player::maxId++;
  this->addModifierInstance(new ModifierInstance(
      GRAVITY_MODIFIER,
      new NumberModifierData(nullptr, Operation::MULTIPLY, GRAVITY_STRENGTH)));
  this->addModifierInstance(new ModifierInstance(
      TAGGED_STATUS_MODIFIER, new TaggedStatusModifierData(nullptr)));
}

core::Player::~Player() {}

void core::Player::tick() { PObject::tick(); }

void core::Player::onTrigger(PObject* other) {
  if (Player* pOther = dynamic_cast<Player*>(other)) {
    vec3f dp = this->getPos() - pOther->getPos();
    float depth = PLAYER_RADIUS * 2 - length(dp);
    this->vel += normalize(dp) * depth / 5.0f;
  }
}
