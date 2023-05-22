#include <catch2/catch_test_macros.hpp>

#include "core/lib.hpp"

TEST_CASE("Simple Effect", "[effect]") {
  Environment* environment = new Environment();
  REQUIRE_NOTHROW(initializeLevel(environment));
  std::pair<Player*, ControlModifierData*> pair1, pair2;
  REQUIRE_NOTHROW(pair1 = initializePlayer());
  REQUIRE_NOTHROW(pair2 = initializePlayer());

  REQUIRE_FALSE(pair1.first->hasModifier(SPEEDBOOST_MODIFIER));
  REQUIRE_FALSE(pair2.first->hasModifier(SPEEDBOOST_MODIFIER));
  SPEEDBOOST_EFFECT->apply(level, pair1.first);
  REQUIRE(pair1.first->hasModifier(SPEEDBOOST_MODIFIER));
  REQUIRE_FALSE(pair2.first->hasModifier(SPEEDBOOST_MODIFIER));

  REQUIRE_NOTHROW(terminateLevel());
}