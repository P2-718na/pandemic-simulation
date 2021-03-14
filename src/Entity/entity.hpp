#pragma once
#include "pathfinder.hpp"

enum EntityStatus {
  still,
  pathing
};

class Entity {
  int _posX;
  int _posY;
  int _status{};
  int _daysInfected{};
  Pathfinder pathfinder{};
  void (*_next)(Entity*);

 public:
  Entity(int posX, int posY, void (*next)(Entity*));
  void moveTo(int destX, int destY);
  int posX() const;
  int posY() const;
  void loop();
};