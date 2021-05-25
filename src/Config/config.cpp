#include <random>

#include "config.hpp"

// I did this to avoid static storage duration warnings
std::default_random_engine & Config::generator_(unsigned long seed) {
  static std::default_random_engine generator{seed};
  return generator;
}

void Config::initRandomGenerator(unsigned long seed) {
  generator_(seed);
}

int Config::randInt(int min, int max) {
  if (min > max) {
    std::swap(min, max);
  }

  std::uniform_int_distribution<int> stepCount(min, max - 1);

  return stepCount(generator_());
}

bool Config::chanceCheck(float chance) {
  return static_cast<float >(randInt(0, 100)) < (100.f * chance);
}

int Config::hourToMinutes(float hour) const noexcept {
  const float minutes = hour / 24.f * static_cast<float>(MINUTES_IN_A_DAY);
  return static_cast<int>(minutes);
}
