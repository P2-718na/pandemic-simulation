#pragma once
#include "ai.hpp"
#include "iworld.hpp"
#include "pathfinder.hpp"

class IWorld;

enum EntityStatus {
  still,
  pathing,
  quarantined
}; //dead

typedef EntityStatus ES;
typedef void (*entityAi)(Entity*, int);

class Entity {
  // Todo check this is set before doing anything
  IWorld* _world{};
  int _uid{};
  int _posX{};
  int _posY{};

  int _daysSinceLastInfection{0};
  bool _quarantined{false};
  bool _infective{false};
  Pathfinder _pathfinder{};
  EntityStatus _status{still};

 public:
  // Ai for next() calls
  // maybe change in queue system?
  entityAi nextAi{AI::nullAi};

  // Variables /////////////////////////////////////////////////////////////////
  // Affects virus symptoms and recovery time
  float virusResistance{.9};

  // Base chance to spread virus to nearby entities
  float virusSpreadChance{.5};

  // Base chance to get infected by virus spread
  float infectionChance{.8};

  // Entity-based POI Coordinates
  std::pair<int, int> homeLocation{-10, -10};
  std::pair<int, int> workLocation{-10, -10};

  // Constructors //////////////////////////////////////////////////////////////
  // Todo Pathfinder will be map-dependant. It will need to be passed by
  //  constructor. Also be sure to pass map by referende.
  //  and implement pathfinder reset method.
  Entity() = default;
  Entity(int uid, int posX, int posY);
  Entity(
    int uid,
    int posX,
    int posY,
    void (*nextAI)(Entity*, int)
  );

  // Accessors /////////////////////////////////////////////////////////////////
  int uid() const;
  int posX() const;
  int posY() const;
  bool infective() const;
  bool quarantined() const;

  void world(IWorld* parent);
  void uid(int uid_);
  void pos(int x, int y);

  // Loops /////////////////////////////////////////////////////////////////////
  // Entity loop, must be run every game loop
  void loop();
  // Entity day loop, must be run every day
  void dayLoop();

  // Methods ///////////////////////////////////////////////////////////////////
  // Load path to destination
  void moveTo(int destX, int destY);
  void moveTo(const std::pair<int, int> &destination);

  // Todo implement methods like goHome(), goWork(), goParty() for AI to call.
  //  The only class to have access to map should be Pathfinder, Entity will
  //  only have access to the coords of these places (or to a set of coords)
  //  and AI will only have access to methods to go to these places.
  //  AI will also have access to time and will be able to specify additional
  //  path metadata, like "fastPath, publicTransport..."
  void goHome(); //todo quarantine logic
  void goWork();
  void goSchool();
  void goWalk();
  void goShop();
  void goParty();

  // Try to infect this entity. Affected by _infectionBaseResistance
  // and _daysSinceLastInfection
  bool tryInfect();

  // Static ////////////////////////////////////////////////////////////////////
  static entityAi parseAi(const std::string &value);
};