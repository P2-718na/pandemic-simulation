#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <string>
#include <SFML/Graphics/Image.hpp>

#include "types.hpp"

class Config;
class Entity;

class World {
  // Current day counter. This will always increment.
  int currentDay_{0};

  // Current minute counter. It will be reset each day.
  int currentMinute_{0};

  // Whether or not we are in a lockdown. Currently lockdown status
  // can only be set manually. It can be improved by having it start
  // automatically after x number of infected people.
  bool lockdown_{false};

  // Background image. We want this to be stored inside World and be private,
  // since we don't want to make changes to this while the simulation
  // is running.
  sf::Image backgroundImage_;

  // Handler to config instance. This can be modified externally by modifying
  // the referenced instance of config.
  Config& config_;

  // Vector containing all entities. Initialized in constructor by
  // parsing entities file.
  std::vector<Entity> entities_;

  // These vectors hold coords for POIs. They are initialized by parsing
  // background image.
  std::vector<Coords> parkCoords_;
  std::vector<Coords> shopCoords_;
  std::vector<Coords> partyCoords_;

  // Private methods ///////////////////////////////////////////////////////////
  // Day loop, called at the end of loop() once every x minutes have passed.
  // Calls dayLoop method for each entity.
  void dayLoop_();

  // Handle virus spread. Called once for each loop() call.
  void spreadVirus_();

  // Handle quarantine logic. Called once for every entity after
  // entity's dayLoop
  void handleQuarantine_(Entity& entity);

 public:
  // Constructors //////////////////////////////////////////////////////////////
  // Initialize world. Loads background image from file and calls
  // parseEntitiesFile() and parsePoiintsOfInteres().
  // Throws a runtime_error if any of these functions fails.
  World(
    const std::string &backgroundImagePath,
    const std::string &entitiesFile,
    Config& config);

  // Getters /////////////////////////////////////////////////////////////////
  // Return current day of the week.
  // Current day is not an enum. This allows us to have weeks with an
  // arbitrary number of days, not exactly 7.
  int currentDay() const noexcept;

  // Return current minute of the day.
  int currentMinute() const noexcept;

  // Return current lockdown status
  bool lockdown() const noexcept;

  // Return reference to world background. This will be used to call draw
  // function on background image.
  const sf::Image& background();

  // Return reference to config.
  const Config& config() const noexcept;

  // Return reference to entities vector.
  const std::vector<Entity>& entities() const noexcept;

  // Return random coordinate from POI lists.
  const Coords& randomParkCoords();
  const Coords& randomShopCoords();
  const Coords& randomPartyCoords();

  // Compute and return infected count.
  int infectedCount() const noexcept;

  // Compute and return dead count.
  int deadCount() const noexcept;

  // Compute immune count by looking at infectionResistance
  int immuneCount() const noexcept;

  // Setters ///////////////////////////////////////////////////////////////////
  // Manually set lockdown status.
  void lockdown(bool status) noexcept;

  // Loop //////////////////////////////////////////////////////////////////////
  // World main loop. Must be called repeatedly to advance the simulation.
  void loop();
};

#endif // define WORLD_HPP
