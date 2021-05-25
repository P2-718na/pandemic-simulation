#include "entity.hpp"
#include "world.hpp"
#include "config.hpp"

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
  return infected_ && daysSinceLastInfection_ > world_->config().DAYS_AFTER_INFECTIVE;
}

// Setters /////////////////////////////////////////////////////////////////////
void Entity::infected(bool status) {
  // Set new status.
  infected_ = status;

  // If this is a new infection, reset daysSinceLastInfection
  if (status) {
    daysSinceLastInfection_ = 0;
    return;
  }

  // If !status, increase infectionResistance.
  this->infectionResistance += (1.f - this->infectionResistance) / world_->config().INFECTION_RESISTANCE_INCREMENT ;
}

void Entity::infective(bool status) {
  // We must call infected first, since infected sets daysSinceLastInfection to 0.
  infected(status);

  if (status) {
    daysSinceLastInfection_ = world_->config().DAYS_AFTER_INFECTIVE + 1;
    return;
  }
}
