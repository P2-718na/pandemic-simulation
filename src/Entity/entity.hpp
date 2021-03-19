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
  Pathfinder pathfinder{};
  void (*_next)(Entity*);

 public:
  Entity(int uid, int posX, int posY);
  Entity(int uid, int posX, int posY, void (*next)(Entity*));

  void moveTo(int destX, int destY);
  int uid() const;
  int posX() const;
  int posY() const;
  void loop();
};