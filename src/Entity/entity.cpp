#include <string>

#include "entity.hpp"
#include "ai.hpp"

// Constructors ////////////////////////////////////////////////////////////////
Entity::Entity(int uid, int posX, int posY)
  : uid_{uid}
  , posX_{posX}
  , posY_{posY}
  , nextAi{AI::nullAi}
{}

Entity::Entity(int uid, int posX, int posY, void (*nextAI)(Entity*, int))
  : Entity(uid, posX, posY)
{
  this->nextAi = nextAI;
}

// Accessors ///////////////////////////////////////////////////////////////////
int Entity::uid() const {
  return this->uid_;
}
int Entity::posX() const {
  return this->posX_;
}
int Entity::posY() const {
  return this->posY_;
}
bool Entity::infective() const {
  // todo add this value to config
  return this->daysSinceLastInfection_ >= 3;
}
bool Entity::quarantined() const {
  return this->quarantined_;
}

void Entity::world(IWorld *parent) {
  this->world_ = parent;
}
void Entity::uid(int uid_) {
  this->uid_ = uid_;
}
void Entity::posX(int x) {
  // This should reset pathfinder, prolly
  this->posX_ = x;
}
void Entity::posY(int y) {
  // This should reset pathfinder, prolly
  this->posY_ = y;
}
void Entity::infective(bool status) {
  // todo this will need to be moved in config

}

// Loops ///////////////////////////////////////////////////////////////////////
// NOTICE this can't be moved to AI since it calls private methods.
void Entity::loop() {
  switch (this->status_) {
    case ES::pathing:
      if (!this->pathfinder_.isArrived()) {
        std::pair<int, int> nextStep = this->pathfinder_.step();
        this->posX_ = nextStep.first;
        this->posY_ = nextStep.second;
      } else {
        this->status_ = still;
      }
      break;

    case ES::still:
      this->nextAi(this, world_->time());
      break;

    // Quarantine status gets applied only when person is already home
    case ES::quarantined:
      // Stay still

    default:
      break;
  }
}

void Entity::dayLoop() {
  // todo reword these if, they can be optimized
  // Change quarantined status. This might need to be moved
  if (
    this->infective() &&
    this->daysSinceLastInfection_ > 2
    ) {
    this->quarantined_ = true;
  }

  // Todo add 10 days min infection time to config
  if (
    this->infective() &&
    this->daysSinceLastInfection_ > 10 &&
    AI::chanceCheck(this->virusResistance)
  ) {
    this->infective(false);
  }

  if (
    this->daysSinceLastInfection_ >= 14 &&
    this->daysSinceLastInfection_ % 7 == 0 &&
    !this->infective()
  ) {
    this->quarantined_ = false;
  }

  if (this->daysSinceLastInfection_ != 0) {
    ++(this->daysSinceLastInfection_);
  }
}

// Methods /////////////////////////////////////////////////////////////////////
// this might need to be a bool in the future, since some paths might be not
// available
void Entity::moveTo(int destX, int destY) {
  this->pathfinder_ = Pathfinder{this->posX_, this->posY_, destX, destY};

  this->status_ = pathing;
}
void Entity::moveTo(const std::pair<int, int> &destination) {
  return this->moveTo(destination.first, destination.second);
}

void Entity::goHome() {
  return this->moveTo(this->homeLocation);
}
void Entity::goWork() {
  return this->moveTo(this->workLocation);
}
void Entity::goSchool() {
  return this->moveTo(this->workLocation);
}
void Entity::goUni() {
  return this->moveTo(this->workLocation);
}
void Entity::goWalk() {
  return this->moveTo(this->world_->randomWalkCoords());
}
void Entity::goShop() {
  return this->moveTo(this->world_->randomShopCoords());
}
void Entity::goParty() {
  return this->moveTo(this->world_->randomPartyCoords());
}

bool Entity::tryInfect() {
  // Do not infect if already infected or if too few time has passed since
  // last infection.
  // Todo move 30 days to config.
  if (
    this->_infective ||
    (this->daysSinceLastInfection_ < 30 && this->daysSinceLastInfection_ != 0)
  ) {
    return false;
  }

  // Do infect if never infected before. Also do infect if more than 2 months
  // have passed since last infection. Infection is still affected by chance.
  if (AI::chanceCheck(this->infectionChance)) {
    this->daysSinceLastInfection_ = 1;
    return this->_infective = true;
  }

  return false;
}

// Static //////////////////////////////////////////////////////////////////////
entityAi Entity::parseAi(const std::string &value) {
  if (value == "nullAi") {
    return AI::nullAi;
  }
  if (value == "randomAi") {
    return AI::randomAi;
  }
  if (value == "testAi") {
    return AI::testAi;
  }

  return AI::testAi;
}
