#include "entity.hpp"
#include "ai.hpp"

// Constructors ////////////////////////////////////////////////////////////////
Entity::Entity(int uid, int posX, int posY)
  : _uid{uid}
  , _posX{posX}
  , _posY{posY}
  , _nextAI(AI::nullAi)
{}

Entity::Entity(int uid, int posX, int posY, void (*nextAI)(Entity*))
  : Entity(uid, posX, posY)
{
  this->_nextAI = nextAI;
}

// Accessors ///////////////////////////////////////////////////////////////////
int Entity::uid() const {
  return this->_uid;
}

int Entity::posX() const {
  return this->_posX;
}

int Entity::posY() const {
  return this->_posY;
}

float Entity::baseSpreadChance() const {
  return this->_virusSpreadChance;
}

bool Entity::infective() const {
  return this->_infective;
}

// Methods /////////////////////////////////////////////////////////////////////
// NOTICE this can't be moved to AI since it calls private methods.
void Entity::loop() {
  switch (this->_status) {
    case pathing:
      if (!this->_pathfinder.isArrived()) {
        std::pair<int, int> nextStep = this->_pathfinder.step();
        this->_posX = nextStep.first;
        this->_posY = nextStep.second;
      } else {
        this->_status = still;
      }
      break;

    case still:
      this->_nextAI(this);
      break;

    default:
      break;
  }
}

void Entity::dayLoop() {
  // Todo add 10 days min infection time to config
  if (
    this->_infective &&
    this->_daysSinceLastInfection > 10 &&
    AI::chanceCheck(this->_virusResistance)
  ) {
    this->_infective = false;
  }

  if (this->_daysSinceLastInfection != 0) {
    ++(this->_daysSinceLastInfection);
  }
}

// this might need to be a bool in the future, since some paths might be not
// available
void Entity::moveTo(int destX, int destY) {
  this->_pathfinder = Pathfinder{this->_posX, this->_posY, destX, destY};

  this->_status = pathing;
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