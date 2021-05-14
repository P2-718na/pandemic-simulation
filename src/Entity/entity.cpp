#include "entity.hpp"

#include <string>

#include "AI/ai.hpp"

// Constructors ////////////////////////////////////////////////////////////////
Entity::Entity(IWorld* world, int uid, int posX, int posY)
  : world_{world}, uid_{uid}, posX_{posX}, posY_{posY} {}

Entity::Entity(IWorld* world, int uid, int posX, int posY, entityAi AI)
  : Entity(world, uid, posX, posY) {
  nextAi_ = AI;
}

// Methods /////////////////////////////////////////////////////////////////////
// this might need to be a bool in the future, since some paths might be not
// available
void Entity::setDestination(int destX, int destY) {
  // Todo this will need to change. maybe .loadDestination method
  //  for pathfinder?

  // Todo pathfinder will need to tell whether or not positiomn is inside grid.
  this->pathfinder_ = Pathfinder{this->posX_, this->posY_, destX, destY};

  this->status_ = pathing;
}
void Entity::setDestination(const Coords& destination) {
  this->setDestination(destination.first, destination.second);
}

void Entity::goHome() {
  this->setDestination(this->homeLocation);
}

void Entity::goWork() {
  this->setDestination(this->workLocation);
}

void Entity::goWalk() {
  this->setDestination(world_->randomParkCoords());
}

void Entity::goShop() {
  this->setDestination(world_->randomShopCoords());
}

void Entity::goParty() {
  this->setDestination(world_->randomPartyCoords());
}

bool Entity::tryInfect() {
  // Do not infect an already infected person.
  // "Removed" people will have their infectionChance go up.
  if (!infected_ && AI::chanceCheck(this->infectionResistance)) {
    this->infected(true);

    // New person infected.
    return true;
  }

  // No new person infected.
  return false;
}
