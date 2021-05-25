#include "world.hpp"
#include "entity.hpp"
#include "config.hpp"

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

  // if infected, handle virus...
  if (infected_) {
    bool resistSymptoms = AI::chanceCheck(this->symptomsResistance);

    // An entity can only die if it starts to show symptoms.
    // aka if it's infective.
    if (this->infective() && !resistSymptoms) {
      if (AI::chanceCheck(world_->config().VIRUS_DEATH_RATE)) {
        dead_ = true;
        return;
      }
    }

    // chance to lose virus.
    if (daysSinceLastInfection_ > world_->config().VIRUS_DURATION && resistSymptoms) {
      this->infected(false);
      return;
    }
  }

  // Increment daysSinceLastInfection only if quarantined or infected
  // since this is used to check for quarantine healing.
  if (quarantined || infected()) {
    ++daysSinceLastInfection_;
  }
}
