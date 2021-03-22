#pragma once
#include "pathfinder.hpp"
#include "iworld.hpp"

class IWorld;

enum EntityStatus { still, pathing, quarantined }; //dead
typedef EntityStatus ES;

class Entity {
  // Todo check this is set before doing anything
  IWorld* _parent{};
  int _uid;
  int _posX;
  int _posY;

  // Ai for next() calls
  // todo maybe change in queue system?
  void (*_nextAI)(Entity*, int);

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
  // Todo Pathfinder will be map-dependant. It will need to be passed by
  //  constructor. Also be sure to pass map by referende.
  //  and implement pathfinder reset method.
  // Todo abstract base class stuff
  Entity(int uid, int posX, int posY);
  Entity(
    int uid,
    int posX,
    int posY,
    void (*nextAI)(Entity*, int)
  );

  // Accessors /////////////////////////////////////////////////////////////////
  void setParent(IWorld* parent);
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

  void goWork();
  void goHome();
  void goShopping();
  void goParty();
  // Todo implement methods like goHome(), goWork(), goParty() for AI to call.
  //  The only class to have access to map should be Pathfinder, Entity will
  //  only have access to the coords of these places (or to a set of coords)
  //  and AI will only have access to methods to go to these places.
  //  AI will also have access to time and will be able to specify additional
  //  path metadata, like "fastPath, publicTransport..."

  // Try to infect this entity. Affected by _infectionBaseResistance
  // and _daysSinceLastInfection
  bool tryInfect();
};