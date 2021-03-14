#include "entity.hpp"
#include "ai.hpp"

Entity::Entity(int posX, int posY, void (*next)(Entity*))
  : _posX{posX}
  , _posY{posY}
  , _next{next}
{}

void Entity::loop() {
  switch (this->_status) {
    case pathing:
      if (!this->pathfinder.isArrived()) {
        int* nextStep = this->pathfinder.step();
        this->_posX = nextStep[0];
        this->_posY = nextStep[1];
      } else {
        this->_status = still;
      }
      break;

    case still:
      this->_next(this);
      break;
    default:
      break;
  }
}

void Entity::moveTo(int destX, int destY) {
  this->pathfinder.init(this->_posX, this->_posY, destX, destY);
  this->_status = pathing;
}