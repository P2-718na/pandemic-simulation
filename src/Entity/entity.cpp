#include "entity.hpp"
#include "ai.hpp"

Entity::Entity(int uid, int posX, int posY)
  : _uid{uid}
  , _posX{posX}
  , _posY{posY}
  , _next(AI::nullAi)
{}

Entity::Entity(int uid, int posX, int posY, void (*next)(Entity*))
  : Entity(uid, posX, posY)
{
  this->_next = next;
}

//todo move this to loop ai file
void Entity::loop() {
  switch (this->_status) {
    case pathing:
      if (!this->pathfinder.isArrived()) {
        std::vector<int> nextStep = this->pathfinder.step();
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

int Entity::uid() const {
  return this->_uid;
}

int Entity::posX() const {
  return this->_posX;
}

int Entity::posY() const {
  return this->_posY;
}

void Entity::moveTo(int destX, int destY) {
  this->pathfinder = Pathfinder{this->_posX, this->_posY, destX, destY};

  this->_status = pathing;
}