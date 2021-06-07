#include "doctest.h"
#include "config.hpp"

using pandemic::Config;

TEST_CASE("Testing Config::hourToMinutes function") {
  Config config;

  CHECK_EQ(config.hourToMinutes(4), 500);
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

TEST_CASE("Check Config::chanceCheck function") {
  const int iterations = 100000;
  float success = 0.f;
  float fail = 0.f;

  SUBCASE("Chance greater than 1") {
    bool good = true;

    for (int i = 0; i < iterations; ++i) {
      if (!Config::chanceCheck(1.1)) {
        good = false;
        break;
      }
    }

    CHECK(good);
  }

  SUBCASE("Chance is 5%") {
    for (int i = 0; i < iterations; ++i) {
      if (Config::chanceCheck(.5)) {
        success++;
        continue;
      }

      fail++;
    }

    CHECK_EQ(success / fail, doctest::Approx(1).epsilon(0.01));
  }

  SUBCASE("Chance is 20%") {
    for (int i = 0; i < iterations; ++i) {
      if (Config::chanceCheck(.2)) {
        success++;
        continue;
      }

      fail++;
    }

    CHECK_EQ(success / fail, doctest::Approx(0.25).epsilon(0.01));
  }

  SUBCASE("Chance is 75%") {
    for (int i = 0; i < iterations; ++i) {
      if (Config::chanceCheck(.75)) {
        success++;
        continue;
      }

      fail++;
    }

    CHECK_EQ(success / fail, doctest::Approx(3).epsilon(0.02));
  }
}
