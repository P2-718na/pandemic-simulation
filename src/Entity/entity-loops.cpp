#include "entity.hpp"

// Loops ///////////////////////////////////////////////////////////////////////
// NOTICE this can't be moved to AI since it calls private methods.
void Entity::loop() {
  switch (status_) {
    case ES::pathing:
      if (!pathfinder_.isArrived()) {
        Coords nextStep = pathfinder_.step();
        posX_ = nextStep.first;
        posY_ = nextStep.second;
      } else {
        status_ = still;
      }
      break;

    case ES::still:
      nextAi_(this, world_->currentMinute());
      break;

      // Quarantine status gets applied only when person is already home
    case ES::quarantined:
      // Stay still

    default:
      break;
  }
}

void Entity::dayLoop() {
  // if quarantined, check every week if it can leave quarantine.
  if (quarantined_ && !infected_ && daysSinceLastInfection_ % 7 == 0) {
    quarantined_ = false;
  }

  // if infected, handle virus...
  if (infected_) {
    // Put person in quarantine after virus gets diagnosed.
    //todo config
    if (daysSinceLastInfection_ > 3) {
      quarantined_ = true;
    }

    // todo config
    // chance to lose virus.
    if (daysSinceLastInfection_ > 7 && AI::chanceCheck(this->symptomsResistance)) {
      this->infected(false);
    }

    ++daysSinceLastInfection_;
  }
}
