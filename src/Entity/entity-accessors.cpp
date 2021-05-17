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

bool Entity::dead() const {
  return dead_;
}

bool Entity::infected() const {
  return infected_;
}

bool Entity::infective() const {
  return daysSinceLastInfection_ > world_->config.DAYS_AFTER_INFECTIVE;
}

// Setters /////////////////////////////////////////////////////////////////////
void Entity::infected(bool status) {
  infected_ = status;

  if (status) {
    daysSinceLastInfection_ = 0;

    return;
  }

  this->infectionResistance += (1.f - this->infectionResistance) / world_->config.INFECTION_RESISTANCE_INCREMENT ;
}

void Entity::infective(bool status) {
  infected(status);

  if (status) {
    daysSinceLastInfection_ = world_->config.DAYS_AFTER_INFECTIVE + 1;
  }
}
