#include "../doctest.h"
#include "config.hpp"

TEST_CASE("Testing Config::hourToMinutes function") {
  Config config;

  config.MINUTES_IN_A_DAY = 9000;
  CHECK_EQ(config.hourToMinutes(5), 1875);
}

TEST_CASE("Check Config::randInt function") {
  const int iterations = 100000;

  SUBCASE("First number is zero") {
    const int low = 0;
    const int high = 10;
    bool good = true;

    for (int i = 0; i < iterations; ++i) {
      int randInt = Config::randInt(low, high);

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
      int randInt = Config::randInt(low, high);

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
      int randInt = Config::randInt(low, high);

      if (randInt < high || randInt >= low) {
        good = false;
        break;
      }
    }

    CHECK(good);
  }
}
