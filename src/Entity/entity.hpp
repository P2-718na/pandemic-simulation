#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>

#include "AI/ai.hpp"
#include "pathfinder.hpp"
#include "types.hpp"

class World;
class Config;

class Entity {
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

  // Pathfinder. Will be changed in the future.
  Pathfinder pathfinder_{};

  // AI of the entity, called every time it reaches the end of its path.
  // This will set the new path.
  // Todo add access to current day for ai.
  AI::entityAI currentAI{AI::nullAI};

  // Convert AI name string to entityAI function pointer.
  static AI::entityAI parseAI_(const std::string &AIName);

 public:
  // fixme move all this variables to private section
  // Infection-related stats of any entity.
  // Affects virus symptoms and recovery time
  float symptomsResistance{.9};
  // Base chance to spread virus to nearby entities
  float virusSpreadChance{.05};
  // Base chance to get infected by virus spread
  float infectionResistance{.8};

  // Entity-based Point of Interest Coordinates
  // workLocation can be work, school or uni location.
  Coords homeLocation{0, 0};
  Coords workLocation{0, 0};

  // Constructors //////////////////////////////////////////////////////////////
  // Todo implement pathfinder reset method.
  // Default entityAi is nullAi.
  Entity(World* world, int uid, int posX, int posY);
  Entity(World* world, int uid, int posX, int posY, const std::string& AIName);

  // Getters ///////////////////////////////////////////////////////////////////
  // fixme should I add noexcept? yes.
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
  void setDestination(int destX, int destY);
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
  // calling Pathfinder::isArrived().
  // If it is, it will call its ai to decide what to do next.
  // If it isn't, it will move forward one tile.
  // If an entity is dead, it will do nothing.
  void loop();

  // Entity day loop, must be run every day.
  void dayLoop();
};

#endif // define ENTITY_HPP
