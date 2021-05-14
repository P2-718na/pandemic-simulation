#include "entity.hpp"

// Getters /////////////////////////////////////////////////////////////////////
int Entity::uid() const {
  return uid_;
}

int Entity::posX() const {
  return posX_;
}

int Entity::posY() const {
  return posY_;
}

Coords Entity::pos() const {
  return {posX_, posY_};
}

int Entity::daysSinceLastInfection() const {
  return daysSinceLastInfection_;
}

bool Entity::quarantined() const {
  return quarantined_;
}

bool Entity::infected() const {
  return infected_;
}

bool Entity::infective() const {
  // todo config
  return daysSinceLastInfection_ > 2;
}

// Setters /////////////////////////////////////////////////////////////////////
void Entity::infected(bool status) {
  infected_ = status;

  if (status) {
    daysSinceLastInfection_ = 0;

    return;
  }

  // todo config
  this->infectionResistance += (1.f - this->infectionResistance) / 1.5f ;
}

void Entity::infective(bool status) {
  infected(status);

  if (status) {
    // todo config
    daysSinceLastInfection_ = 99;
  }
}
