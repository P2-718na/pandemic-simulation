#include "Entity/AI/ai.hpp"

#include "../dist/doctest.h"

TEST_CASE("Check AI::randInt function") {
  const int iterations = 100000;

  SUBCASE("First number is zero") {
    const int low = 0;
    const int high = 10;
    bool good = true;

    for (int i = 0; i < iterations; ++i) {
      int randInt = AI::randInt(low, high);

      if (randInt < low || randInt >= high) {
        good = false;
        break;
      }
    }

    CHECK(good);
  }

  SUBCASE("Numbers are not zero") {
    const int low = 11;
    const int high = 23;
    bool good = true;

    for (int i = 0; i < iterations; ++i) {
      int randInt = AI::randInt(low, high);

      if (randInt < low || randInt >= high) {
        good = false;
        break;
      }
    }

    CHECK(good);
  }

  SUBCASE("Min greater than max") {
    const int low = 44;
    const int high = 23;
    bool good = true;

    for (int i = 0; i < iterations; ++i) {
      int randInt = AI::randInt(low, high);

      if (randInt < high || randInt >= low) {
        good = false;
        break;
      }
    }

    CHECK(good);
  }
}
