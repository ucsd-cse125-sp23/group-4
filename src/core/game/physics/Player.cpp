#include "core/game/physics/Player.h"

#include "core/util/global.h"

uint32_t Player::maxId = 1;

Player::Player()
    : PObject(PLAYER_BOUNDING_SHAPE, PLAYER_LAYER, 0, false),
      look(vec3f(1.0f, 0.0f, 0.0f)) {
  this->pid = Player::maxId++;
  this->addModifierInstance(new ModifierInstance(
      GRAVITY_MODIFIER,
      new NumberModifierData(nullptr, Operation::MULTIPLY, GRAVITY_STRENGTH)));
  this->addModifierInstance(new ModifierInstance(
      TAGGED_STATUS_MODIFIER, new TaggedStatusModifierData(nullptr)));
}

Player::~Player() {}

void Player::tick() { PObject::tick(); }
