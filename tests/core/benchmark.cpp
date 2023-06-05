#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <limits>

#include "core/lib.hpp"

struct Time {
  double t;
};
bool operator<(const Time& t0, const Time& t1) { return t0.t < t1.t; }
bool operator>(const Time& t0, const Time& t1) { return t0.t > t1.t; }
void operator+=(Time& t0, const Time& t1) { t0.t += t1.t; }
Time operator/(const Time& t0, const Time& t1) { return {t0.t / t1.t}; }
std::ostream& operator<<(std::ostream& os, const Time& time) {
  if (time.t * 1e3 < 1)
    os << std::setprecision(3) << (time.t * 1e6) << " us";
  else if (time.t < 1)
    os << std::setprecision(3) << (time.t * 1e3) << " ms";
  else
    os << std::setprecision(3) << (time.t) << " s";
  return os;
}

struct Timer {
  std::chrono::time_point<std::chrono::system_clock> last;
};
inline Time tick(Timer& timer) {
  std::chrono::time_point<std::chrono::system_clock> now =
      std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = now - timer.last;
  double ret = elapsed.count();
  timer.last = now;
  return {ret};
}

void testEnvironment(Environment* environment, int S, int P = 1) {
  Level* level;
  REQUIRE_NOTHROW(level = initializeLevel(environment));
  std::vector<std::pair<Player*, ControlModifierData*>> pairs;
  for (int i = 0; i < P; i++)
    REQUIRE_NOTHROW(pairs.push_back(initializePlayer()));

  Timer timer;
  Time minT = {std::numeric_limits<double>::max()},
       maxT = {std::numeric_limits<double>::min()}, sum = {0};
  for (int i = 0; i < S; i++) {
    for (auto pair : pairs) pair.first->setPos(vec3f(0.0, 0.9, 0.0));
    tick(timer);
    level->tick();
    Time time = tick(timer);
    if (time < minT) minT = time;
    if (time > maxT) maxT = time;
    sum += time;
  }
  std::cout << S
            << " samples: (avg = " << (sum / Time({static_cast<double>(S)}))
            << ", min = " << minT << ", max = " << maxT << ")" << std::endl;

  REQUIRE_NOTHROW(terminateLevel());
}
TEST_CASE("Single Object Multi Collider", "[benchmark]") {
  SECTION("100 AABB") {
    Environment* environment = new Environment();
    for (int i = 0; i < 100; i++)
      environment->addBox(vec3f(-1, -1, -1), vec3f(1, 1, 1));
    std::cout << "100 AABBs for ";
    testEnvironment(environment, 20);
  }
  SECTION("1000 AABB") {
    Environment* environment = new Environment();
    for (int i = 0; i < 1000; i++)
      environment->addBox(vec3f(-1, -1, -1), vec3f(1, 1, 1));
    std::cout << "1000 AABBs for ";
    testEnvironment(environment, 20);
  }
  std::vector<vec3f> vertices;
  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++)
      for (int k = 0; k < 10; k++)
        vertices.push_back(vec3f((i - 5.0) / 5, (j - 5.0) / 5, (k - 5.0) / 5));
  SECTION("100 Convex of 1000 vertices") {
    Environment* environment = new Environment();
    for (int i = 0; i < 100; i++) environment->addConvex(vertices);
    std::cout << "100 Convexs of 1000 vertices for ";
    testEnvironment(environment, 20);
  }

  vertices.clear();
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 20; j++)
      for (int k = 0; k < 20; k++)
        vertices.push_back(
            vec3f((i - 10.0) / 10, (j - 10.0) / 10, (k - 10.0) / 10));
  SECTION("100 Convex of 8000 vertices") {
    Environment* environment = new Environment();
    for (int i = 0; i < 100; i++) environment->addConvex(vertices);
    std::cout << "100 Convexs of 8000 vertices for ";
    testEnvironment(environment, 20);
  }
}

TEST_CASE("Single Object Multi Collider Spreadout", "[benchmark]") {
  SECTION("100 AABB (spread out)") {
    Environment* environment = new Environment();
    for (int x = 0; x < 10; x++)
      for (int z = 0; z < 10; z++)
        environment->addBox(vec3f(-1 + (x - 5), -1, -1 + (z - 5)),
                            vec3f(1 + (x - 5), 1, 1 - (z - 5)));
    std::cout << "100 AABBs (spread out) for ";
    testEnvironment(environment, 20);
  }
  SECTION("1600 AABB (spread out)") {
    Environment* environment = new Environment();
    for (int x = 0; x < 40; x++)
      for (int z = 0; z < 40; z++)
        environment->addBox(vec3f(-1 + (x - 20), -1, -1 + (z - 20)),
                            vec3f(1 + (x - 20), 1, 1 - (z - 20)));
    std::cout << "1600 AABBs (spread out) for ";
    testEnvironment(environment, 20);
  }
  std::vector<vec3f> vertices;
  SECTION("100 Convex of 1000 vertices (spread out)") {
    Environment* environment = new Environment();
    for (int x = 0; x < 10; x++)
      for (int z = 0; z < 10; z++) {
        for (int i = 0; i < 10; i++)
          for (int j = 0; j < 10; j++)
            for (int k = 0; k < 10; k++)
              vertices.push_back(vec3f((i - 5.0) / 5 + (x - 5), (j - 5.0) / 5,
                                       (k - 5.0) / 5 + (z - 5)));
        environment->addConvex(vertices);
        vertices.clear();
      }
    std::cout << "100 Convexs of 1000 vertices (spread out) for ";
    testEnvironment(environment, 20);
  }
  SECTION("1600 Convex of 1000 vertices (spread out)") {
    Environment* environment = new Environment();
    for (int x = 0; x < 40; x++)
      for (int z = 0; z < 40; z++) {
        for (int i = 0; i < 10; i++)
          for (int j = 0; j < 10; j++)
            for (int k = 0; k < 10; k++)
              vertices.push_back(vec3f((i - 5.0) / 5 + (x - 20), (j - 5.0) / 5,
                                       (k - 5.0) / 5 + (z - 20)));
        environment->addConvex(vertices);
        vertices.clear();
      }
    std::cout << "1600 Convexs of 1000 vertices (spread out) for ";
    testEnvironment(environment, 20);
  }

  SECTION("100 Convex of 8000 vertices (spread out)") {
    Environment* environment = new Environment();
    for (int x = 0; x < 10; x++)
      for (int z = 0; z < 10; z++) {
        for (int i = 0; i < 20; i++)
          for (int j = 0; j < 20; j++)
            for (int k = 0; k < 20; k++)
              vertices.push_back(vec3f((i - 10.0) / 10 + (x - 5),
                                       (j - 10.0) / 10,
                                       (k - 10.0) / 10 + (z - 5)));
        environment->addConvex(vertices);
        vertices.clear();
      }
    std::cout << "100 Convexs of 8000 vertices (spread out) for ";
    testEnvironment(environment, 20);
  }
  SECTION("1600 Convex of 8000 vertices (spread out)") {
    Environment* environment = new Environment();
    for (int x = 0; x < 40; x++)
      for (int z = 0; z < 40; z++) {
        for (int i = 0; i < 20; i++)
          for (int j = 0; j < 20; j++)
            for (int k = 0; k < 20; k++)
              vertices.push_back(vec3f((i - 10.0) / 10 + (x - 20),
                                       (j - 10.0) / 10,
                                       (k - 10.0) / 10 + (z - 20)));
        environment->addConvex(vertices);
        vertices.clear();
      }
    std::cout << "1600 Convexs of 8000 vertices (spread out) for ";
    testEnvironment(environment, 20);
  }
}

TEST_CASE("Multi Object Multi Collider Spreadout", "[benchmark]") {
  std::vector<vec3f> vertices;
  SECTION("2 Player with 1600 Convex of 1000 vertices (spread out)") {
    Environment* environment = new Environment();
    for (int x = 0; x < 40; x++)
      for (int z = 0; z < 40; z++) {
        for (int i = 0; i < 10; i++)
          for (int j = 0; j < 10; j++)
            for (int k = 0; k < 10; k++)
              vertices.push_back(vec3f((i - 5.0) / 5 + (x - 20), (j - 5.0) / 5,
                                       (k - 5.0) / 5 + (z - 20)));
        environment->addConvex(vertices);
        vertices.clear();
      }
    std::cout
        << "2 Player with 1600 Convexs of 1000 vertices (spread out) for ";
    testEnvironment(environment, 20, 2);
  }
  SECTION("4 Player with 1600 Convex of 1000 vertices (spread out)") {
    Environment* environment = new Environment();
    for (int x = 0; x < 40; x++)
      for (int z = 0; z < 40; z++) {
        for (int i = 0; i < 10; i++)
          for (int j = 0; j < 10; j++)
            for (int k = 0; k < 10; k++)
              vertices.push_back(vec3f((i - 5.0) / 5 + (x - 20), (j - 5.0) / 5,
                                       (k - 5.0) / 5 + (z - 20)));
        environment->addConvex(vertices);
        vertices.clear();
      }
    std::cout
        << "4 Player with 1600 Convexs of 1000 vertices (spread out) for ";
    testEnvironment(environment, 20, 4);
  }
}
