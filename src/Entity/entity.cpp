#include <string>
#include <cassert>

#include "world.hpp"
#include "entity.hpp"
#include "config.hpp"
#include "AI/variants/ai-variants.hpp"

namespace pandemic {

// Constructors ////////////////////////////////////////////////////////////////
Entity::Entity(
  World* world, int uid, int posX, int posY, const std::string& AIName)
  : world_{world}, uid_{uid}, posX_{posX}, posY_{posY}, AI_{parseAI_(AIName)} {}

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

float Entity::virusSpreadChance() const noexcept {
  return virusSpreadChance_;
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
  return infected_ && daysSinceLastInfection_ > config().DAYS_AFTER_INFECTIVE();
}

bool Entity::quarantined() const noexcept {
  return quarantined_;
}

bool Entity::immune() const noexcept {
  return infectionResistance_ >= 1.f;
}

// Setters /////////////////////////////////////////////////////////////////////
bool Entity::symptomsResistance(float value) noexcept {
  if (value < 0) {
    return false;
  }

  symptomsResistance_ = value;
  return true;
}

bool Entity::virusSpreadChance(float value) noexcept {
  if (value < 0) {
    return false;
  }

  virusSpreadChance_ = value;
  return true;
}

bool Entity::infectionResistance(float value) noexcept {
  if (value < 0) {
    return false;
  }

  infectionResistance_ = value;
  return true;
}

void Entity::workLocation(Coords value) noexcept {
  workLocation_ = std::move(value);
}

void Entity::homeLocation(Coords value) noexcept {
  homeLocation_ = std::move(value);
}

void Entity::infected(bool status) {
  // InfectionResistance is only assigned when a person defeats virus.
  // Since a person can onluy get virus if infRes < 1, infRes here must be
  // lesser than 1.
  assert(infectionResistance_ < 1);

  // Set new status.
  infected_ = status;

  // If this is a new infection, reset daysSinceLastInfection
  if (status) {
    daysSinceLastInfection_ = 0;
    return;
  }

  // If infection is cleared, increase infectionResistance_.
  infectionResistance_ += config().INFECTION_RESISTANCE_INCREMENT();
}

void Entity::infective(bool status) {
  // We must call infected first, since infected sets daysSinceLastInfection to 0.
  infected(status);

  // Make sure that all calls to infective() will return true.
  if (status) {
    daysSinceLastInfection_ = config().DAYS_AFTER_INFECTIVE() + 1;
    return;
  }
}

void Entity::quarantined(bool status) {
  quarantined_ = status;
}

// Methods /////////////////////////////////////////////////////////////////////
void Entity::setDestination(const Coords& destination) {
  // Avoid unnecessary pathfinder calls if we are already at position.
  if (pos() == destination) {
    return;
  }

  // todo this is bad. Make function to check if coords are valid
  // If the destinations is world::invalidLocation, stay still.
  if (destination == world_->invalidCoords()) {
    return;
  }

  // Otherwise, reset pathfinder.
  pathfinder_.loadPath(pos(), destination);
}

void Entity::goHome() {
  setDestination(homeLocation_);
}

void Entity::goWork() {
  setDestination(workLocation_);
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
  // Notice that we need to check for 1-infResistance, since a person with
  // infResistance of 1 will have zero chance of being infected
  if (!infected() && Config::chanceCheck(1 - infectionResistance_)) {
    // If infRes >= 1, it means that something went wrong with chanceCheck
    // function.
    assert(infectionResistance_ < 1);

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
  if (pathfinder_.arrived()) {
    (*AI_)(world_->currentMinute(), world_->currentDay());
    return;
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
    bool resistSymptoms = Config::chanceCheck(symptomsResistance_);

    // An entity can only die if it starts to show symptoms.
    // aka if it's infective.
    if (infective() && !resistSymptoms) {
      if (Config::chanceCheck(config().VIRUS_DEATH_RATE())) {
        dead_ = true;
        return;
      }
    }

    // chance to lose virus.
    if (daysSinceLastInfection_ > config().VIRUS_DURATION() && resistSymptoms) {
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
  // Default AIs
  if (AIName == "nullAI") {
    return std::make_unique<nullAI>(this);
  }
  if (AIName == "randomAI") {
    return std::make_unique<randomAI>(this);
  }

  // AI Variants
  if (AIName == "manAI") {
    return std::make_unique<manAI>(this);
  }
  if (AIName == "oldAI") {
    return std::make_unique<oldAI>(this);
  }
  if (AIName == "gradAI") {
    return std::make_unique<gradAI>(this);
  }
  if (AIName == "uniAI") {
    return std::make_unique<uniAI>(this);
  }
  if (AIName == "teenAI") {
    return std::make_unique<teenAI>(this);
  }

  return std::make_unique<nullAI>(this);
}

} // namespace pandemic
