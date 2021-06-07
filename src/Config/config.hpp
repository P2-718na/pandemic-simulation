#ifndef PANDEMIC_CONFIG_HPP
#define PANDEMIC_CONFIG_HPP

#include <SFML/Graphics/Color.hpp>
#include <random>

namespace pandemic {

// This class holds all the configurable variables for the simulation.
// For now, it is mostly a placeholder, but it can be expanded in many
// ways.
// E.g. load and write configuration from file, edit simulation parameters
// while the simulation is running, etc...
class Config {
  // random engine is a static variable in this class. This was done
  // to avoid static initialization warnings.
  // This way, the generator will be initialised the first time
  // this function is called.
  static std::default_random_engine& generator_();

  // Config values /////////////////////////////////////////////////////////////
  // POI Colors
  const sf::Color PARK_COLOR_{0x00, 0xff, 0x00};
  const sf::Color SHOP_COLOR_{0xff, 0xff, 0x00};
  const sf::Color PARTY_COLOR_{0xff, 0x00, 0xff};

  // Number of minutes after every day loop.
  int MINUTES_IN_A_DAY_{3000};

  // Number of days in a week. Must be an integer greater than 3
  // (required by AIs to work correctly)
  int DAYS_IN_A_WEEK_{7};

  // Day count after which infected entity becomes infective. This is checked
  // using ">", which means the actual number of days is this value + 1.
  int DAYS_AFTER_INFECTIVE_{2};

  // Days after which an infected person goes into quarantine.
  int DAYS_AFTER_QUARANTINE_{3};

  // Interval to check if virus is defeated and if entity can leave
  // quarantine.
  int QUARANTINE_CHECK_INTERVAL_{7};

  // Infection resistance increment after defeating the virus.
  float INFECTION_RESISTANCE_INCREMENT_{.4f};

  // Every day, if an infected entity loses a virus symptoms check,
  // it performs a VIRUS_DEATH_RATE check
  float VIRUS_DEATH_RATE_{0.05f};

  // Number of days after which an entity can recover from the virus.
  int VIRUS_DURATION_{7};

 public:
  // Constructor ///////////////////////////////////////////////////////////////
  // Initialize generator to catch errors.
  Config();

  // Utils /////////////////////////////////////////////////////////////////////
  // Generate random int in [min, max[.
  static int randInt(int min, int max) noexcept;

  // Return true "chance" percent of the times.
  static bool chanceCheck(float chance) noexcept;

  // Return current "hour", given current minute.
  int hourToMinutes(float hour) const noexcept;

  // Getters ///////////////////////////////////////////////////////////////////
  const sf::Color& PARK_COLOR() const noexcept;
  const sf::Color& SHOP_COLOR() const noexcept;
  const sf::Color& PARTY_COLOR() const noexcept;
  int MINUTES_IN_A_DAY() const noexcept;
  int DAYS_IN_A_WEEK() const noexcept;
  int DAYS_AFTER_INFECTIVE() const noexcept;
  int DAYS_AFTER_QUARANTINE() const noexcept;
  int QUARANTINE_CHECK_INTERVAL() const noexcept;
  float INFECTION_RESISTANCE_INCREMENT() const noexcept;
  float VIRUS_DEATH_RATE() const noexcept;
  int VIRUS_DURATION() const noexcept;

};

} // namespace pandemic

#endif // define PANDEMIC_CONFIG_HPP
