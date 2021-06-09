#include <random>
#include <utility>

#include "config.hpp"

namespace pandemic {

// Constructor /////////////////////////////////////////////////////////////////
Config::Config() {
  // Initialize generator. This way we can check for errors the first time
  // config is instantiated. This is useless for every other instantiation,
  // but it doesn't really affect the program in any way.
  generator_();
}

// Utils ///////////////////////////////////////////////////////////////////////
std::default_random_engine& Config::generator_() {
  static std::random_device rd;
  static std::default_random_engine generator{rd()};
  return generator;
}

int Config::randInt(int min, int max) noexcept {
  if (min > max) {
    std::swap(min, max);
  }

  std::uniform_int_distribution<int> distrib(min, max - 1);

  return distrib(generator_());
}

bool Config::chanceCheck(float chance) noexcept {
  return static_cast<float>(randInt(0, 100)) < (100.f * chance);
}

int Config::hourToMinutes(float hour) const noexcept {
  const float minutes = hour / 24.f * static_cast<float>(MINUTES_IN_A_DAY());
  return static_cast<int>(minutes);
}

// Getters /////////////////////////////////////////////////////////////////////
const sf::Color& Config::PARK_COLOR() const noexcept {
  return PARK_COLOR_;
}

const sf::Color& Config::SHOP_COLOR() const noexcept {
  return SHOP_COLOR_;
}

const sf::Color& Config::PARTY_COLOR() const noexcept {
  return PARTY_COLOR_;
}

int Config::MINUTES_IN_A_DAY() const noexcept {
  return MINUTES_IN_A_DAY_;
}

int Config::DAYS_IN_A_WEEK() const noexcept {
  return DAYS_IN_A_WEEK_;
}

int Config::DAYS_AFTER_INFECTIVE() const noexcept {
  return DAYS_AFTER_INFECTIVE_;
}

int Config::DAYS_AFTER_QUARANTINE() const noexcept {
  return DAYS_AFTER_QUARANTINE_;
}

int Config::QUARANTINE_CHECK_INTERVAL() const noexcept {
  return QUARANTINE_CHECK_INTERVAL_;
}

float Config::INFECTION_RESISTANCE_INCREMENT() const noexcept {
  return INFECTION_RESISTANCE_INCREMENT_;
}

float Config::VIRUS_DEATH_RATE() const noexcept {
  return VIRUS_DEATH_RATE_;
}

int Config::VIRUS_DURATION() const noexcept {
  return VIRUS_DURATION_;
}

}  // namespace pandemic
