#pragma once
#include "pathfinder.hpp"

enum EntityStatus {
  still,
  pathing
};

class Entity {
  int _uid;
  int _posX;
  int _posY;
  int _status{};
  void (*_nextAI)(Entity*);

  Pathfinder pathfinder{};

 public:
  // todo actual function .infected()
  int daysInfected{0};

  // Constructors
  Entity(int uid, int posX, int posY);
  Entity(int uid, int posX, int posY, void (*nextAI)(Entity*));

  void moveTo(int destX, int destY);
  int uid() const;
  int posX() const;
  int posY() const;
  void loop();
};