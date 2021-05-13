#pragma once
#include <string>

#include "Entity/AI/ai.hpp"
#include "iworld.hpp"
#include "pathfinder.hpp"

class IWorld;

enum EntityStatus
{
  still,
  pathing,
  quarantined,
  dead
};

typedef EntityStatus ES;
// Coordinates
typedef std::pair<int, int> Coords;
// Entity*, points to current entity, int is the current time of day.
typedef void (*entityAi)(Entity*, int);

class Entity {
  // Todo check this is set before doing anything
  //  move to constructor
  IWorld* world_{};
  int uid_{};
  int posX_{};
  int posY_{};

  int daysSinceLastInfection_{0};
  bool quarantined_{false};
  bool infected_{false};

  // Todo this will become a shared pointer
  Pathfinder pathfinder_{};
  EntityStatus status_{still};
  // Ai for next() calls
  // maybe change in queue system?
  entityAi nextAi_{AI::nullAi};

 public:
  // Variables /////////////////////////////////////////////////////////////////
  // Affects virus symptoms and recovery time
  float symptomsResistance{.9};
  // Base chance to spread virus to nearby entities
  float virusSpreadChance{.5};
  // Base chance to get infected by virus spread
  float infectionResistance{.8};

  // Entity-based POI Coordinates
  // workLocation can be work, school or uni location.
  Coords homeLocation{0, 0};
  Coords workLocation{0, 0};

  // Constructors //////////////////////////////////////////////////////////////
  // Todo Pathfinder will be map-dependant.
  //  implement pathfinder reset method and add pathfinder in constructor.
  Entity(IWorld* world, int uid, int posX, int posY);
  Entity(IWorld* world, int uid, int posX, int posY, entityAi AI);

  // Getters ///////////////////////////////////////////////////////////////////
  int uid() const;
  int posX() const;
  int posY() const;
  int daysSinceLastInfection() const;
  bool quarantined() const;
  bool infected() const;
  bool infective() const;

  // Setters ///////////////////////////////////////////////////////////////////
  // This should be used in initialization and by private members.
  // Sets daysSinceLastInfection and infective.
  // If a person is not infected anymore, adds some infectionResistance.
  void infected(bool status);

  // Methods ///////////////////////////////////////////////////////////////////
  // Load path to destination.
  void setDestination(int destX, int destY);
  void setDestination(const Coords& destination);

  // Todo
  //  AI will also have access to time and will be able to specify additional
  //  path metadata, like "fastPath, publicTransport..."
  void goHome();  // todo quarantine logic
  void goWork();
  void goWalk();
  void goShop();
  void goParty();

  // Try to infect this entity. Affected by _infectionBaseResistance
  // and _daysSinceLastInfection
  bool tryInfect();

  // Loops /////////////////////////////////////////////////////////////////////
  // Entity loop, must be run every game loop
  void loop();
  // Entity day loop, must be run every day
  void dayLoop();
};
