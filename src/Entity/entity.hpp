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

  // Infection-related stats of any entity.
  // Chance to resist virus symptoms. Affects probability of death
  // and recovery time.
  float symptomsResistance_{.9};
  // Chance to spread virus to nearby entities.
  float virusSpreadChance_{.05};
  // Chance to get infected by virus spread
  float infectionResistance_{.8};

  // Entity-based Point of Interest Coordinates
  // workLocation_ can represent work, school or uni location. It's up to
  // each entity's ai to use this accordingly (e.g. teens go to school
  // only for five hours a day, so their AI will call Entity::goWork()
  // for just five hours in the morning.
  Coords workLocation_{0, 0};
  Coords homeLocation_{0, 0};

  // Days since infection. Updated at the end of each dayLoop.
  // The value is rounded up (e.g., if an entity is infected in the last
  // minute of day 1, as soon as day 2 begins, this value will be 1).
  int daysSinceLastInfection_{0};

  // Dead status. Dead entities will do nothing and will not spread the virus.
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
  // This is a pointer, since AI use class inheritance to work.
  entityAI aiPtr_;

  // Convert AI name string to entityAI function pointer.
  entityAI parseAI_(const std::string& AIName);

 public:
  // Constructors //////////////////////////////////////////////////////////////
  // Default entityAi is nullAi.
  Entity(World* world, int uid, int posX, int posY,
    const std::string& AIName = "nullAI");

  // Getters ///////////////////////////////////////////////////////////////////
  const Config& config() const noexcept;
  int uid() const noexcept;
  int posX() const noexcept;
  int posY() const noexcept;
  Coords pos() const noexcept;
  float virusSpreadChance() const noexcept;
  int daysSinceLastInfection() const noexcept;
  bool dead() const noexcept;
  bool infected() const noexcept;
  bool infective() const noexcept;
  bool quarantined() const noexcept;
  bool immune() const noexcept;

  // Setters ///////////////////////////////////////////////////////////////////
  // These methods are used only in entity initialization and for internal
  // logic. Having these methods public makes it easier to add additional
  // functionality in the future (example: increase infection resistance
  // globally to simulate the use of face masks).

  // if value is >= 0, these functions return true and change the
  // parameter accordingly. Otherwise, they return false and leave
  // the parameter unchanged
  bool symptomsResistance(float value) noexcept;
  bool virusSpreadChance(float value) noexcept;
  bool infectionResistance(float value) noexcept;

  // Checking that these coordinates are valid is up to whoever initialises
  // the entity
  void workLocation(Coords value) noexcept;
  void homeLocation(Coords value) noexcept;

  // Sets daysSinceLastInfection and infective.
  // If a person is not infected anymore, adds some infectionResistance_.
  void infected(bool status) noexcept;

  // calls infected(true) and sets daysSinceLastInfection.
  // This should be used only in constructor.
  void infective(bool status) noexcept;

  // Sets quarantined to true
  void quarantined(bool status) noexcept;

  // Methods ///////////////////////////////////////////////////////////////////
  // Load path to destination. Calls pathfinder.
  void setDestination(const Coords& destination) noexcept;

  void goHome() noexcept;
  void goWork() noexcept;
  void goWalk() noexcept;
  void goShop() noexcept;
  void goParty() noexcept;

  // Try to infect this entity. Affected by infectionResistance_.
  bool tryInfect() noexcept;

  // Loops /////////////////////////////////////////////////////////////////////
  // Entity loop, must be run every game loop.
  // Checks whether or not an entity is arrived to its set destination by
  // calling Pathfinder::arrived().
  // If it is, it will call its ai to decide what to do next.
  // If it isn't, it will move forward one tile.
  // If an entity is dead, it will do nothing.
  void loop() noexcept;

  // Entity day loop, must be run every day.
  void dayLoop() noexcept;
};

}  // namespace pandemic

#endif  // define PANDEMIC_ENTITY_HPP
