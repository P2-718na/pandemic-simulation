#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <string>

#include "Entity/AI/ai.hpp"
#include "pathfinder.hpp"

typedef std::pair<int, int> Coords;

class World;

// Entity*, points to current entity, int is the current time of day.
typedef void (*entityAi)(Entity*, int);

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
  // todo add death logic.
  bool dead_{false};

  // Infected status. If this is true, every day loop we check if entity can
  // defeat the virus.
  // Note that infected and infective are two separate conditions.
  bool infected_{false};

  // Pathfinder. Will be changed in the future.
  // TODO shared pointer to global pathfinder instance
  Pathfinder pathfinder_{};

  // AI of the entity, called every time it reaches the end of its path.
  // This will set the new path.
  entityAi nextAi_{AI::nullAI};

 public:
  // Quarantined status. Public, since this is regulated by outside
  // factors.
  // Todo move quarantine logic to WORLD
  bool quarantined{false};

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
  // Todo Pathfinder will be map-dependant.
  //  implement pathfinder reset method and add pathfinder in constructor.
  // Default entityAi is nullAi.
  Entity(World* world, int uid, int posX, int posY);
  Entity(World* world, int uid, int posX, int posY, entityAi AI);

  // Getters ///////////////////////////////////////////////////////////////////
  // fixme should I add noexcept?
  int uid() const;
  int posX() const;
  int posY() const;
  Coords pos() const;
  int daysSinceLastInfection() const;
  bool dead() const;
  bool infected() const;
  bool infective() const;

  // Setters ///////////////////////////////////////////////////////////////////
  // This should be used in initialization and by private members.
  // Sets daysSinceLastInfection and infective.
  // If a person is not infected anymore, adds some infectionResistance.
  void infected(bool status);

  // calls infected(true) and sets daysSinceLastInfection.
  // This should be used only in constructor.
  void infective(bool status);

  // Methods ///////////////////////////////////////////////////////////////////
  // Load path to destination.
  void setDestination(int destX, int destY);
  void setDestination(const Coords& destination);

  // Todo add access to current day for ai.
  void goHome();  // todo quarantine logic
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
