#include "entity.hpp"
#include "ai.hpp"

// Constructors ////////////////////////////////////////////////////////////////
Entity::Entity(int uid, int posX, int posY)
  : _uid{uid}
  , _posX{posX}
  , _posY{posY}
  , _nextAI{AI::nullAi}
{}

Entity::Entity(int uid, int posX, int posY, void (*nextAI)(Entity*, int))
  : Entity(uid, posX, posY)
{
  this->_nextAI = nextAI;
}

// Accessors ///////////////////////////////////////////////////////////////////
void Entity::setParent(IWorld *parent) {
  this->_world = parent;
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

float Entity::virusSpreadChance() const {
  return this->_virusSpreadChance;
}

bool Entity::infective() const {
  return this->_infective;
}

bool Entity::quarantined() const {
  return this->_quarantined;
}

// Loops ///////////////////////////////////////////////////////////////////////
// NOTICE this can't be moved to AI since it calls private methods.
void Entity::loop() {
  switch (this->_status) {
    case ES::pathing:
      if (!this->_pathfinder.isArrived()) {
        std::pair<int, int> nextStep = this->_pathfinder.step();
        this->_posX = nextStep.first;
        this->_posY = nextStep.second;
      } else {
        this->_status = still;
      }
      break;

    case ES::still:
      this->_nextAI(this, _world->time());
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
    this->_daysSinceLastInfection > 2
    ) {
    this->_quarantined = true;
  }

  // Todo add 10 days min infection time to config
  if (
    this->infective() &&
    this->_daysSinceLastInfection > 10 &&
    AI::chanceCheck(this->_virusResistance)
  ) {
    this->_infective = false;
  }

  if (
    this->_daysSinceLastInfection >= 14 &&
    this->_daysSinceLastInfection % 7 == 0 &&
    !this->infective()
  ) {
    this->_quarantined = false;
  }

  if (this->_daysSinceLastInfection != 0) {
    ++(this->_daysSinceLastInfection);
  }
}

// Methods /////////////////////////////////////////////////////////////////////
// this might need to be a bool in the future, since some paths might be not
// available
void Entity::moveTo(int destX, int destY) {
  this->_pathfinder = Pathfinder{this->_posX, this->_posY, destX, destY};

  this->_status = pathing;
}
void Entity::moveTo(const std::pair<int, int> &destination) {
  return this->moveTo(destination.first, destination.second);
}

void Entity::goHome() {
  return this->moveTo(this->_homeLocation);
}
void Entity::goWork() {
  return this->moveTo(this->_workLocation);
}
void Entity::goSchool() {
  return this->moveTo(this->_schoolLocation);
}
void Entity::goWalk() {
  return this->moveTo(this->_world->randomWalkCoords());
}
void Entity::goShop() {
  return this->moveTo(this->_world->randomShopCoords());
}
void Entity::goParty() {
  return this->moveTo(this->_world->randomPartyCoords());
}

bool Entity::tryInfect() {
  // Do not infect if already infected or if too few time has passed since
  // last infection.
  // Todo move 30 days to config.
  if (
    this->_infective ||
    (this->_daysSinceLastInfection < 30 && this->_daysSinceLastInfection != 0)
  ) {
    return false;
  }

  // Do infect if never infected before. Also do infect if more than 2 months
  // have passed since last infection. Infection is still affected by chance.
  if (AI::chanceCheck(this->_infectionChance)) {
    this->_daysSinceLastInfection = 1;
    return this->_infective = true;
  }

  return false;
}