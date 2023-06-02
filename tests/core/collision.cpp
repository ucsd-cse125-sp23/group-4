#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "core/math/matrix.h"
#include "core/math/shape/AABShape.h"
#include "core/math/shape/CapsuleShape.h"
#include "core/math/shape/OffsetShape.h"
#include "core/math/shape/SphereShape.h"
#include "core/math/vector.h"

using Catch::Matchers::WithinAbs;

TEST_CASE("Trivial Collision Detection", "[detect]") {
  SphereShape* sphere = new SphereShape(1);
  REQUIRE(sphere->collides(sphere));
  delete sphere;
}
TEST_CASE("Sphere Collision Detection", "[detect]") {
  SphereShape* sphere = new SphereShape(1);
  CHECK_FALSE(sphere->collides(sphere, translate(vec3f(1, 1, 1)),
                               translate(vec3f(-1, -1, -1))));
  CHECK_FALSE(sphere->collides(sphere, translate(vec3f(2, 0, 0))));
  CHECK(sphere->collides(sphere, translate(vec3f(1, 0, 0))));
  CHECK(sphere->collides(sphere, translate(vec3f(0, 1, 0))));
  CHECK(sphere->collides(sphere, translate(vec3f(0, 0, 1))));
  CHECK(sphere->collides(sphere, translate(vec3f(-1, 0, 0))));
  CHECK(sphere->collides(sphere, translate(vec3f(0, -1, 0))));
  CHECK(sphere->collides(sphere, translate(vec3f(0, 0, -1))));
  CHECK_FALSE(sphere->collides(
      sphere, translate(vec3f(0.0, 0.0, 1.5)) * scale(vec3f(0.4, 0.4, 0.4))));
  CHECK_FALSE(sphere->collides(sphere, translate(vec3f(0.0, 0.0, 1.5)),
                               scale(vec3f(0.4, 0.4, 0.4))));
  delete sphere;
}
/*
TEST_CASE("Cylinder Collision Detection", "[detect]") {
  CylinderShape* cylinder = new CylinderShape(2, 1);
  CHECK(cylinder->collides(cylinder));
  CHECK_FALSE(cylinder->collides(cylinder, translate(vec3f(1, 1, 1)),
                                 translate(vec3f(-1, -1, -1))));
  CHECK_FALSE(cylinder->collides(cylinder, translate(vec3f(2.01, 0.0, 0.0))));
  CHECK_FALSE(cylinder->collides(cylinder, translate(vec3f(0.0, 2.01, 0.0))));
  CHECK_FALSE(cylinder->collides(cylinder, translate(vec3f(0.0, 0.0, 2.01))));
  CHECK_FALSE(cylinder->collides(cylinder, translate(vec3f(-2.01, 0.0, 0.0))));
  CHECK_FALSE(cylinder->collides(cylinder, translate(vec3f(0.0, -2.01, 0.0))));
  CHECK_FALSE(cylinder->collides(cylinder, translate(vec3f(0.0, 0.0, -2.01))));
  CHECK(cylinder->collides(cylinder, translate(vec3f(1, 0, 0))));
  CHECK(cylinder->collides(cylinder, translate(vec3f(0, 1, 0))));
  CHECK(cylinder->collides(cylinder, translate(vec3f(0, 0, 1))));
  CHECK(cylinder->collides(cylinder, translate(vec3f(-1, 0, 0))));
  CHECK(cylinder->collides(cylinder, translate(vec3f(0, -1, 0))));
  CHECK(cylinder->collides(cylinder, translate(vec3f(0, 0, -1))));
  CHECK_FALSE(cylinder->collides(
      cylinder, translate(vec3f(0.0, 0.0, 1.5)) * scale(vec3f(0.4, 0.4, 0.4))));
  CHECK_FALSE(cylinder->collides(cylinder, translate(vec3f(0.0, 0.0, 1.5)),
                                 scale(vec3f(0.4, 0.4, 0.4))));
  delete cylinder;
}*/
TEST_CASE("AABB Collision Detection", "[detect]") {
  AABShape* aab1 = new AABShape(vec3f(-1, -1, -1), vec3f(1, 1, 1));
  CHECK(aab1->collides(aab1));
  CHECK_FALSE(aab1->collides(aab1, translate(vec3f(1, 1, 1)),
                             translate(vec3f(-1, -1, -1))));
  CHECK_FALSE(aab1->collides(aab1, translate(vec3f(2.01, 0.0, 0.0))));
  CHECK_FALSE(aab1->collides(aab1, translate(vec3f(0.0, 2.01, 0.0))));
  CHECK_FALSE(aab1->collides(aab1, translate(vec3f(0.0, 0.0, 2.01))));
  CHECK_FALSE(aab1->collides(aab1, translate(vec3f(-2.01, 0.0, 0.0))));
  CHECK_FALSE(aab1->collides(aab1, translate(vec3f(0.0, -2.01, 0.0))));
  CHECK_FALSE(aab1->collides(aab1, translate(vec3f(0.0, 0.0, -2.01))));
  CHECK(aab1->collides(aab1, translate(vec3f(1, 0, 0))));
  CHECK(aab1->collides(aab1, translate(vec3f(0, 1, 0))));
  CHECK(aab1->collides(aab1, translate(vec3f(0, 0, 1))));
  CHECK(aab1->collides(aab1, translate(vec3f(-1, 0, 0))));
  CHECK(aab1->collides(aab1, translate(vec3f(0, -1, 0))));
  CHECK(aab1->collides(aab1, translate(vec3f(0, 0, -1))));
  CHECK_FALSE(aab1->collides(
      aab1, translate(vec3f(0.0, 0.0, 1.5)) * scale(vec3f(0.4, 0.4, 0.4))));
  CHECK_FALSE(aab1->collides(aab1, translate(vec3f(0.0, 0.0, 1.5)),
                             scale(vec3f(0.4, 0.4, 0.4))));
  AABShape* aab2 = new AABShape(vec3f(2, 2, 2), vec3f(3, 3, 3));
  CHECK_FALSE(aab1->collides(aab2));
  CHECK(aab1->collides(aab2, translate(vec3f(1.01, 1.01, 1.01))));
  delete aab1;
  delete aab2;
}
/*
TEST_CASE("Transform Collision Detection", "[detect]") {
  CylinderShape* cylinder = new CylinderShape(2.0, 0.5);
  CHECK_FALSE(cylinder->collides(cylinder, translate(vec3f(1.01, 0.00, 0.00))));
  CHECK_FALSE(cylinder->collides(cylinder, rotate(vec3f(0.00, 1.57, 0.00)),
                                 translate(vec3f(0.00, 1.51, 0.00))));
  delete cylinder;
}
TEST_CASE("Mixed Collision Detection", "[detect]") {
  CylinderShape* cylinder = new CylinderShape(2.0, 0.5);
  AABShape* aab = new AABShape(vec3f(-1, -1, -1), vec3f(1, 1, 1));
  CHECK(cylinder->collides(aab, translate(vec3f(1.00, 0.00, 0.00))));
  CHECK(cylinder->collides(aab, translate(vec3f(1.00, 1.00, 0.00))));
  CHECK(cylinder->collides(aab, translate(vec3f(1.00, -2.00, 1.00))));
  CHECK_FALSE(cylinder->collides(aab, translate(vec3f(2.00, 0.00, 2.00))));
  CHECK_FALSE(cylinder->collides(aab, translate(vec3f(1.00, 3.01, 0.00))));
  CHECK_FALSE(cylinder->collides(aab, translate(vec3f(0.00, 0.00, 2.51))));
  delete cylinder;
  delete aab;
}
*/
void test_mtv(BoundingShape* shape, vec3f dx, vec4f expected,
              float shiftEpsilon = 0.001f,
              vec4f tolerance = vec4f(0.01f, 0.01f, 0.01f, 0.001f)) {
  vec4f mtv = shape->mtv(shape, translate(dx));
  CHECK(shape->collides(shape, translate(dx)));
  CHECK_THAT(mtv.w, WithinAbs(expected.w, tolerance.w));
  CHECK_THAT(mtv.x, WithinAbs(expected.x, tolerance.x));
  CHECK_THAT(mtv.y, WithinAbs(expected.y, tolerance.y));
  CHECK_THAT(mtv.z, WithinAbs(expected.z, tolerance.z));
  CHECK_FALSE(shape->collides(
      shape, translate(dx + vec3f(mtv) * (mtv.w + shiftEpsilon))));
}
void test_mtv(BoundingShape* shape0, BoundingShape* shape1, vec3f dx,
              vec4f expected, float shiftEpsilon = 0.001f,
              vec4f tolerance = vec4f(0.01f, 0.01f, 0.01f, 0.001f)) {
  vec4f mtv = shape0->mtv(shape1, translate(dx));
  CHECK(shape0->collides(shape1, translate(dx)));
  CHECK_THAT(mtv.w, WithinAbs(expected.w, tolerance.w));
  CHECK_THAT(mtv.x, WithinAbs(expected.x, tolerance.x));
  CHECK_THAT(mtv.y, WithinAbs(expected.y, tolerance.y));
  CHECK_THAT(mtv.z, WithinAbs(expected.z, tolerance.z));
  CHECK_FALSE(shape0->collides(
      shape1, translate(dx + vec3f(mtv) * (mtv.w + shiftEpsilon))));
}
TEST_CASE("Trivial Collision MTV", "[mtv]") {
  SphereShape* sphere = new SphereShape(1);
  vec4f mtv = sphere->mtv(sphere, translate(vec3f(1, 0, 0)));
  REQUIRE(sphere->collides(sphere, translate(vec3f(1, 0, 0))));
  REQUIRE_FALSE(sphere->collides(
      sphere, translate(vec3f(1, 0, 0) + vec3f(mtv) * (mtv.w + 0.001f))));
  delete sphere;
}
/*
TEST_CASE("Cylinder Collision MTV", "[mtv]") {
  CylinderShape* cylinder = new CylinderShape(2.0, 0.5);
  test_mtv(cylinder, vec3f(0.9, 0.0, 0.0), vec4f(1.0, 0.0, 0.0, 0.1), 0.001f,
           vec4f(0.02, 0.02, 0.02, 0.001));
  test_mtv(cylinder, vec3f(0.6, 0.0, 0.6), vec4f(0.707, 0.0, 0.707, 0.1515),
           0.001f);
  test_mtv(cylinder, vec3f(0.6, 0.3, 0.6), vec4f(0.707, 0.0, 0.707, 0.1515),
           0.001f);
  test_mtv(cylinder, vec3f(0.1, 1.7, 0.1), vec4f(0.0, 1.0, 0.0, 0.3), 0.001f);
  delete cylinder;
}
TEST_CASE("Mixed Collision MTV", "[mtv]") {
  CylinderShape* cylinder = new CylinderShape(2.0, 0.5);
  AABShape* aab = new AABShape(vec3f(-1, -3, -1), vec3f(1, 3, 1));
  test_mtv(cylinder, aab, vec3f(1.4, 0.0, 0.0), vec4f(1.0, 0.0, 0.0, 0.1),
           0.001f);
  test_mtv(cylinder, aab, vec3f(1.3, 0.0, 1.2), vec4f(0.827, 0.0, 0.561, 0.139),
           0.001f);
  test_mtv(cylinder, aab, vec3f(1.3, 0.8, 1.2), vec4f(0.827, 0.0, 0.561, 0.139),
           0.001f);
  test_mtv(cylinder, aab, vec3f(0.1, 3.8, 0.1), vec4f(0.0, 1.0, 0.0, 0.2),
           0.001f);
  delete cylinder;
  delete aab;
}
TEST_CASE("Mixed Offset Collision MTV", "[mtv]") {
  CylinderShape* cylinder = new CylinderShape(1.0, 0.5);
  OffsetShape* offset = new OffsetShape(cylinder, vec3f(0.0f, 0.5f, 0.0f));
  AABShape* aab = new AABShape(vec3f(-2, -4, -2), vec3f(2, -2, 2));
  test_mtv(cylinder, aab, vec3f(0.0, -1.6, 0.0), vec4f(0.0, 1.0, 0.0, 0.1),
           0.001f);
  test_mtv(offset, aab, vec3f(0.0, -2.2, 0.0), vec4f(0.0, 1.0, 0.0, 0.2),
           0.001f);
  delete cylinder;
  delete offset;
  delete aab;
}
*/
TEST_CASE("Simple Point Test", "[pointtest]") {
  SphereShape* sphere = new SphereShape(1);
  CHECK(sphere->contains(vec3f(0, 0, 0)));
  CHECK(sphere->contains(vec3f(0.0, 0.5, 0.0)));
  CHECK(sphere->contains(vec3f(0.0, 0.5, 0.5)));
  CHECK(sphere->contains(vec3f(0.5, 0.5, 0.5)));

  CHECK_FALSE(sphere->contains(vec3f(0, 0, 0), translate(vec3f(1, 1, 1))));
  CHECK(sphere->contains(vec3f(1, 1, 1), translate(vec3f(1, 1, 1))));
  delete sphere;

  AABShape* aab = new AABShape(vec3f(-1, -1, -1), vec3f(1, 1, 1));
  CHECK(aab->contains(vec3f(0.9, 0.9, -0.9)));
  CHECK(aab->contains(vec3f(0.9, 0.9, 0.9)));
  CHECK(aab->contains(vec3f(0.9, -0.9, -0.9)));
  CHECK(aab->contains(vec3f(0.9, -0.9, 0.9)));
  CHECK(aab->contains(vec3f(-0.9, 0.9, -0.9)));
  CHECK(aab->contains(vec3f(-0.9, 0.9, 0.9)));
  CHECK(aab->contains(vec3f(-0.9, -0.9, -0.9)));
  CHECK(aab->contains(vec3f(-0.9, -0.9, 0.9)));
  delete aab;
}

TEST_CASE("Simple Raycast", "[raycast]") {
  SphereShape* sphere = new SphereShape(1);
  CHECK_THAT(sphere->intersects({vec3f(0, 0, 0), vec3f(1, 0, 0)},
                                translate(vec3f(5, 0, 0))),
             WithinAbs(4, 0.01f));
  CHECK_THAT(sphere->intersects({vec3f(0, 0, 0), vec3f(-1, 0, 0)},
                                translate(vec3f(5, 0, 0))),
             WithinAbs(-1, 0.01f));
  CHECK_THAT(sphere->intersects({vec3f(0, 0, 0), vec3f(1, 0, 0)},
                                translate(vec3f(5.0, 0.9, 0.0))),
             WithinAbs(4.56, 0.01f));
  delete sphere;

  AABShape* aab = new AABShape(vec3f(-1, -1, -1), vec3f(1, 1, 1));
  CHECK_THAT(aab->intersects({vec3f(0, 0, 0), vec3f(1, 0, 0)},
                             translate(vec3f(5, 0, 0))),
             WithinAbs(4, 0.01f));
  CHECK_THAT(aab->intersects({vec3f(0.0, 0.5, 0.5), vec3f(1, 0, 0)},
                             translate(vec3f(5, 0, 0))),
             WithinAbs(4, 0.01f));
  CHECK_THAT(aab->intersects({vec3f(0, 0, 0), vec3f(1.0, 0.2, 0.2)},
                             translate(vec3f(5, 0, 0))),
             WithinAbs(4, 0.01f));
  delete aab;
}

#include "core/lib.hpp"

TEST_CASE("Basic CCD", "[ccd]") {
  PLAYER_BASE_SHAPE = new CapsuleShape(1.0, 1.3);
  PLAYER_BOUNDING_SHAPE =
      new OffsetShape(PLAYER_BASE_SHAPE, vec3f(0.0f, 0.5f, 0.0f));

  Environment* environment = new Environment();
  environment->addBox(vec3f(-2000, -2, -2000), vec3f(2000, 0, 2000));
  REQUIRE_NOTHROW(initializeLevel(environment));
  std::pair<Player*, ControlModifierData*> pair;
  REQUIRE_NOTHROW(pair = initializePlayer());

  pair.first->setPos(vec3f(0, 1, 0));

  CHECK_THAT(level->getEnvironment()->ccd(pair.first, vec3f(0, -4, 0)).second.w,
             WithinAbs(0.25f, 0.01f));
  CHECK_THAT(level->getEnvironment()->ccd(pair.first, vec3f(1, -4, 0)).second.w,
             WithinAbs(0.25f, 0.01f));
  CHECK_THAT(level->getEnvironment()->ccd(pair.first, vec3f(1, -4, 2)).second.w,
             WithinAbs(0.25f, 0.01f));
  CHECK_THAT(
      level->getEnvironment()->ccd(pair.first, vec3f(50, -4, 0)).second.w,
             WithinAbs(0.25f, 0.01f));


  REQUIRE_NOTHROW(terminateLevel());
}
void checkCCD(PObject* obj, vec3f dPos) {
  float t = level->getEnvironment()->ccd(obj, dPos).second.w;
  obj->addPos((dPos - normalize(dPos) * 0.001f) * t);
  CHECK(level->getEnvironment()->collides(obj).empty());
}
TEST_CASE("Multi Obsticle CCD", "[ccd]") {
  PLAYER_BOUNDING_SHAPE =
      new OffsetShape(new SphereShape(1.0), vec3f(0.0f, 0.0f, 0.0f));

  Environment* environment = new Environment();
  environment->addBox(vec3f(-200, -2, -200), vec3f(200, 0, 200));
  environment->addBox(vec3f(-2, -200, -200), vec3f(0, 200, 200));
  REQUIRE_NOTHROW(initializeLevel(environment));
  std::pair<Player*, ControlModifierData*> pair;
  REQUIRE_NOTHROW(pair = initializePlayer());

  pair.first->setPos(vec3f(2.0, 2.0, 0.0));
  checkCCD(pair.first, vec3f(0, -4, 0));

  pair.first->setPos(vec3f(2.0, 2.0, 0.0));
  checkCCD(pair.first, vec3f(-4, -4, 0));

  pair.first->setPos(vec3f(2.0, 2.0, 0.0));
  checkCCD(pair.first, vec3f(-8, 0, 0));

  pair.first->setPos(vec3f(2.0, 2.0, 0.0));
  CHECK_THAT(level->getEnvironment()->ccd(pair.first, vec3f(0, -2, 50)).second.w, WithinAbs(0.5,0.001));

  REQUIRE_NOTHROW(terminateLevel());
}