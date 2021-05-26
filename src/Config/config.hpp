#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <SFML/Graphics/Color.hpp>
#include <random>

class Config {
  // Initialize random number generator_
  static std::default_random_engine& generator_();

 public:
  static void initRandomGenerator();

  // Utils /////////////////////////////////////////////////////////////////////
  // Generate random int in [min, max[.
  static int randInt(int min, int max);

  // Return true "chance" percent of the times.
  static bool chanceCheck(float chance);

  // Return current "hour", given current minute.
  int hourToMinutes(float hour) const noexcept;

  // todo these probably need to be changed. We want some error handling on
  //  these values.
  // Config values /////////////////////////////////////////////////////////////
  // POI Colors
  sf::Color PARK_COLOR{0x00, 0xff, 0x00};
  sf::Color SHOP_COLOR{0xff, 0xff, 0x00};
  sf::Color PARTY_COLOR{0xff, 0x00, 0xff};

  // Number of minutes after every day loop.
  int MINUTES_IN_A_DAY{2400};

  // Number of days in a week.
  int DAYS_IN_A_WEEK{7};

  // Day count after which infected entity becomes infective. This is checked
  // using ">", which means the actual number of days is this value + 1.
  int DAYS_AFTER_INFECTIVE{2};

  // Days after which an infected person goes into quarantine.
  int DAYS_AFTER_QUARANTINE{3};

  // Interval to check if virus is defeated and entity can leave
  // quarantine.
  int QUARANTINE_CHECK_INTERVAL{7};

  // Infection resistance increment after defeating the virus.
  float INFECTION_RESISTANCE_INCREMENT{.4f};

  // Every day, if an infected entity loses a virus symptoms check,
  // it performs a VIRUS_DEATH_RATE check
  float VIRUS_DEATH_RATE{0.05f};

  // Number of days after which an entity can recover from the virus.
  int VIRUS_DURATION{7};

  // todo loadFromFile
  // bool loadFromFile(string filename);
};

#endif // define CONFIG_HPP
