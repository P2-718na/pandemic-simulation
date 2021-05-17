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
  if (quarantined && !infected_ && daysSinceLastInfection_ % world_->config.QUARANTINE_CHECK_INTERVAL == 0) {
    quarantined = false;
  }

  // if infected, handle virus...
  if (infected_) {
    // Put person in quarantine after virus gets diagnosed.
    if (daysSinceLastInfection_ > world_->config.DAYS_AFTER_QUARANTINE) {

      // Todo move quarantine logic to WORLD
      quarantined = true;
    }

    bool resistSymptoms = AI::chanceCheck(this->symptomsResistance);

    if (this->infective() && !resistSymptoms) {
      if (AI::chanceCheck(world_->config.VIRUS_DEATH_RATE)) {
        dead_ = true;
        return;
      }
    }

    // chance to lose virus.
    if (daysSinceLastInfection_ > world_->config.VIRUS_DURATION && resistSymptoms) {
      this->infected(false);
    }

    ++daysSinceLastInfection_;
  }
}
