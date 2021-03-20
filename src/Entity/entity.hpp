#pragma once
#include "pathfinder.hpp"

enum EntityStatus { still, pathing }; //dead

class Entity {
  int _uid;
  int _posX;
  int _posY;

  // Ai for next() calls
  // todo maybe change in queue system?
  void (*_nextAI)(Entity*);

  // Affects virus symptoms and recovery time
  float _virusBaseResistance{.9};

  // Base chance to spread virus to nearby entities
  float _baseSpreadChance{.5};

  // Base chance to get infected by virus spread
  float _baseInfectionChance{.8};

  int _age{18};

  Pathfinder _pathfinder{};
  EntityStatus _status{still};
  int _daysSinceLastInfection{0};
  bool _infective{false};

 public:
  // Constructors //////////////////////////////////////////////////////////////
  Entity(int uid, int posX, int posY);
  Entity(int uid, int posX, int posY, void (*nextAI)(Entity*));

  // Accessors /////////////////////////////////////////////////////////////////
  int uid() const;
  int posX() const;
  int posY() const;
  bool isInfective() const;

  // Methods ///////////////////////////////////////////////////////////////////
  // Entity loop, must be run every game loop
  void loop();
  // Entity day loop, must be run every day
  void dayLoop();

  // Load path to destination
  void moveTo(int destX, int destY);

  // Try to infect this entity. Affected by _infectionBaseResistance
  // and _daysSinceLastInfection
  bool tryInfect();
};