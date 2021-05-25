#include "entity.hpp"
#include "world.hpp"
#include "config.hpp"

#include <string>

#include "AI/ai.hpp"

// Constructors ////////////////////////////////////////////////////////////////
Entity::Entity(World* world, int uid, int posX, int posY)
  : world_{world}, uid_{uid}, posX_{posX}, posY_{posY} {}

Entity::Entity(
  World* world, int uid, int posX, int posY, const std::string& AIName)
  : Entity(world, uid, posX, posY) {
  nextAi_ = parseAI_(AIName);
}

// Getters /////////////////////////////////////////////////////////////////////
const Config& Entity::config() const noexcept {
  return world_->config();
}

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
  return infected_ && daysSinceLastInfection_ > config().DAYS_AFTER_INFECTIVE;
}

bool Entity::quarantined() const noexcept {
  return quarantined_;
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

  // If infection is cleared, increase infectionResistance.
  infectionResistance += (1.f - infectionResistance) / config().INFECTION_RESISTANCE_INCREMENT ;
}

void Entity::infective(bool status) {
  // We must call infected first, since infected sets daysSinceLastInfection to 0.
  infected(status);

  // Make sure that all calls to infective() will return true.
  if (status) {
    daysSinceLastInfection_ = config().DAYS_AFTER_INFECTIVE + 1;
    return;
  }
}

void Entity::quarantined(bool status) {
  quarantined_ = status;
}

// Methods /////////////////////////////////////////////////////////////////////
void Entity::setDestination(int destX, int destY) {
  // Todo pathfinder will need to tell whether or not position is inside grid.
  pathfinder_ = Pathfinder{posX_, posY_, destX, destY};
}
void Entity::setDestination(const Coords& destination) {
  setDestination(destination.first, destination.second);
}

void Entity::goHome() {
  setDestination(homeLocation);
}

void Entity::goWork() {
  setDestination(workLocation);
}

void Entity::goWalk() {
  setDestination(world_->randomParkCoords());
}

void Entity::goShop() {
  setDestination(world_->randomShopCoords());
}

void Entity::goParty() {
  setDestination(world_->randomPartyCoords());
}

bool Entity::tryInfect() {
  // Do not infect an already infected person. (It is required to check this,
  // since infected() will reset daysSinceLastInfection).
  // "Removed" people will have their infectionChance go up.
  if (!infected() && Config::chanceCheck(infectionResistance)) {
    infected(true);

    // New person infected.
    return true;
  }

  // No new person infected.
  return false;
}

// Loops ///////////////////////////////////////////////////////////////////////
void Entity::loop() {
  // Do nothing if dead.
  if (dead()) {
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
  if (dead()) {
    return;
  }

  // if infected, handle virus...
  if (infected()) {
    bool resistSymptoms = Config::chanceCheck(symptomsResistance);

    // An entity can only die if it starts to show symptoms.
    // aka if it's infective.
    if (infective() && !resistSymptoms) {
      if (Config::chanceCheck(config().VIRUS_DEATH_RATE)) {
        dead_ = true;
        return;
      }
    }

    // chance to lose virus.
    if (daysSinceLastInfection_ > config().VIRUS_DURATION && resistSymptoms) {
      infected(false);
      return;
    }
  }

  // Increment daysSinceLastInfection only if quarantined or infected
  // since this is used to check for quarantine healing.
  if (quarantined() || infected()) {
    ++daysSinceLastInfection_;
  }
}

// Other ///////////////////////////////////////////////////////////////////////
entityAI Entity::parseAI_(const std::string& AIName) {
  if (AIName == "nullAI") {
    return AI::nullAI;
  }
  if (AIName == "randomAI") {
    return AI::randomAI;
  }
  if (AIName == "testAI") {
    return AI::testAI;
  }

  return AI::testAI;
}
