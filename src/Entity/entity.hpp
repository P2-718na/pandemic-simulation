#ifndef PANDEMIC_ENTITY_HPP
#define PANDEMIC_ENTITY_HPP

#include <string>
#include <memory>

#include "AI/ai.hpp"
#include "pathfinder.hpp"
#include "types.hpp"

namespace pandemic {

class World;
class Config;

class Entity {
  friend class AI;

  // World this entity belongs to.
  World* world_;

  // uid of entity. It is not checked whether or not this is actually
  // unique. Currently unused, kept mainly for debugging purposes.
  int uid_;

  // Current X and Y position of entity.
  int posX_;
  int posY_;

  // Days since infection. Updated at the end of each dayLoop.
  // The value is rounded up (e.g., if an entity is infected in the last
  // minute of day 1, as soon as day 2 begins, this value will be 1).
  int daysSinceLastInfection_{0};

  // Dead status.
  bool dead_{false};

  // Infected status. If this is true, every day loop we check if entity can
  // defeat the virus.
  // Note that infected and infective are two separate conditions.
  bool infected_{false};

  // Quarantined status.
  bool quarantined_{false};

  // Pathfinder instance. Initialize with an arbitrary size to store paths.
  Pathfinder pathfinder_{1000};

  // AI of the entity, called every time it reaches the end of its path.
  // This will set the new path.
  entityAI AI_;

  // Convert AI name string to entityAI function pointer.
  entityAI parseAI_(const std::string& AIName);

 public:
  // fixme move all this variables to private section
  // Infection-related stats of any entity.
  // Affects virus symptoms and recovery time.
  // todo justify float and not double
  float symptomsResistance{.9};
  // Base chance to spread virus to nearby entities
  float virusSpreadChance{.05};
  // Base chance to get infected by virus spread
  float infectionResistance{.8};

  // Entity-based Point of Interest Coordinates
  // workLocation can represent work, school or uni location.
  Coords homeLocation{0, 0};
  Coords workLocation{0, 0};

  // Constructors //////////////////////////////////////////////////////////////
  // Default entityAi is nullAi.
  Entity(World* world, int uid, int posX, int posY,
    const std::string& AIName = "nullAI");

  // Getters ///////////////////////////////////////////////////////////////////
  // todo add noexcept to everything
  const Config& config() const noexcept;
  int uid() const;
  int posX() const;
  int posY() const;
  Coords pos() const;
  int daysSinceLastInfection() const;
  bool dead() const;
  bool infected() const;
  bool infective() const;
  bool quarantined() const noexcept;

  // Setters ///////////////////////////////////////////////////////////////////
  // This should be used in initialization and by private members.
  // Sets daysSinceLastInfection and infective.
  // If a person is not infected anymore, adds some infectionResistance.
  void infected(bool status);

  // calls infected(true) and sets daysSinceLastInfection.
  // This should be used only in constructor.
  void infective(bool status);

  // Sets quarantined to true
  void quarantined(bool status);

  // Methods ///////////////////////////////////////////////////////////////////
  // Load path to destination. Calls pathfinder.
  void setDestination(const Coords& destination);

  void goHome();
  void goWork();
  void goWalk();
  void goShop();
  void goParty();

  // Try to infect this entity. Affected by infectionResistance.
  bool tryInfect();

  // Loops /////////////////////////////////////////////////////////////////////
  // Entity loop, must be run every game loop.
  // Checks whether or not an entity is arrived to its set destination by
  // calling Pathfinder::arrived().
  // If it is, it will call its ai to decide what to do next.
  // If it isn't, it will move forward one tile.
  // If an entity is dead, it will do nothing.
  void loop();

  // Entity day loop, must be run every day.
  void dayLoop();
};

}  // namespace pandemic

#endif  // define PANDEMIC_ENTITY_HPP
