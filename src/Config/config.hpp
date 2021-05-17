#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <SFML/Graphics/Color.hpp>

class Config {
 public:
  // POI Colors
  sf::Color PARK_COLOR{0x00, 0xff, 0x00};
  sf::Color SHOP_COLOR{0xff, 0xff, 0x00};
  sf::Color PARTY_COLOR{0xff, 0x00, 0xff};

  // Number of minutes after every day loop.
  int MINUTES_IN_A_DAY{9000};

  // Day count after which infected entity becomes infective. This is checked
  // using ">", which means the actual number of days is this value + 1.
  int DAYS_AFTER_INFECTIVE{2};

  // Days after which an infected person goes into quarantine
  int DAYS_AFTER_QUARANTINE{3};

  // Infection resistance increment after defeating the virus.
  // Calculated with (1.f - infResistance) / INFECTION_RESISTANCE_INCREMENT;
  float INFECTION_RESISTANCE_INCREMENT{1.5f};

  // Every day, if an infected entity loses a virus symptoms check,
  // it performs a VIRUS_DEATH_RATE check
  float VIRUS_DEATH_RATE{0.1f};

  // Number of days after which an entity can recover from the virus.
  int VIRUS_DURATION{7};

  // todo loadFromFile
  // bool loadFromFile(string filename);
};

#endif // define CONFIG_HPP
