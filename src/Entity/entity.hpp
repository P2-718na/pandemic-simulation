#pragma once
#include "pathfinder.hpp"

enum EntityStatus { still, pathing, quarantined }; //dead
typedef EntityStatus ES;

class Entity {
  int _uid;
  int _posX;
  int _posY;

  // Ai for next() calls
  // todo maybe change in queue system?
  void (*_nextAI)(Entity*);

  // Affects virus symptoms and recovery time
  float _virusResistance{.9};

  // Base chance to spread virus to nearby entities
  float _virusSpreadChance{.5};

  // Base chance to get infected by virus spread
  float _infectionChance{.8};

  int _daysSinceLastInfection{0};
  bool _quarantined{false};
  bool _infective{false};
  Pathfinder _pathfinder{};
  EntityStatus _status{still};

  // Home coordinates
  std::pair<int, int> _homeLocation{-10, -10};

 public:
  // Constructors //////////////////////////////////////////////////////////////
  Entity(int uid, int posX, int posY);
  Entity(int uid, int posX, int posY, void (*nextAI)(Entity*));

  // Accessors /////////////////////////////////////////////////////////////////
  int uid() const;
  int posX() const;
  int posY() const;
  float virusSpreadChance() const;
  bool infective() const;
  bool quarantined() const;

  // Methods ///////////////////////////////////////////////////////////////////
  // Entity loop, must be run every game loop
  void loop();
  // Entity day loop, must be run every day
  void dayLoop();

  // Load path to destination
  void moveTo(int destX, int destY);

  // Go home and stay quarantined
  void goHome();

  // Try to infect this entity. Affected by _infectionBaseResistance
  // and _daysSinceLastInfection
  bool tryInfect();
};