#include "entity.hpp"

// Loops ///////////////////////////////////////////////////////////////////////
void Entity::loop() {
  // Do nothing if dead.
  if (dead_) {
    return;
  }

  // IF arrived to destination, call AI
  if (pathfinder_.isArrived()) {
    return nextAi_(this, world_->currentMinute());
  }

  // Else, move one tile.
  Coords nextStep = pathfinder_.step();
  posX_ = nextStep.first;
  posY_ = nextStep.second;
}

void Entity::dayLoop() {
  // Do nothing if dead.
  if (dead_) {
    return;
  }

  // Todo move quarantine logic to WORLD
  // if quarantined, check every week if it can leave quarantine.
  if (quarantined && !infected_ && daysSinceLastInfection_ % 7 == 0) {
    quarantined = false;
  }

  // if infected, handle virus...
  if (infected_) {
    // Put person in quarantine after virus gets diagnosed.
    //todo config
    if (daysSinceLastInfection_ > 3) {

      // Todo move quarantine logic to WORLD
      quarantined = true;
    }

    bool resistSymptoms = AI::chanceCheck(this->symptomsResistance);

    if (!resistSymptoms) {
      // todo config. This is virus death rate
      if (AI::chanceCheck(0.1)) {
        dead_ = true;
        return;
      }
    }

    // todo config
    // chance to lose virus.
    if (daysSinceLastInfection_ > 7 && resistSymptoms) {
      this->infected(false);
    }

    ++daysSinceLastInfection_;
  }
}
