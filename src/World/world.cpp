#include <cassert>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>

#include "Parser/parser.hpp"
#include "config.hpp"
#include "entity.hpp"
#include "world.hpp"

namespace pandemic {

// Constructor /////////////////////////////////////////////////////////////////
World::World(
  const std::string& backgroundImagePath, const std::string& entitiesFilePath,
  const Config& config)
  : config_{ config } {
  // (See invalidCoords_ comment)
  assert(!validPosition(invalidCoords_));

  // Load background_ image
  if (!backgroundImage_.loadFromFile(backgroundImagePath)) {
    throw std::runtime_error("Cannot load image from file.");
  }

  // Load entities and points of interest
  try {
    Parser::parseEntitiesFile(this, entitiesFilePath, entities_);
    Parser::parsePointsOfInterests(
      config,
      backgroundImage_,
      parkCoords_,
      shopCoords_,
      partyCoords_);
  } catch (...) {
    std::cerr << "Error initialising world." << std::endl;
    throw;
  }
}

// Loops ///////////////////////////////////////////////////////////////////////
void World::dayLoop_() {
  // Increment day counter
  ++currentDay_;
  // and reset minute counter.
  currentMinute_ = 0;

  // For every entity, run its dayLoop and handle quarantine logic.
  for (auto& entity : entities_) {
    entity.dayLoop();
    handleQuarantine_(entity);
  }
}

void World::loop() {
  // Increment minute counter
  ++currentMinute_;

  // Loop every entity
  for (auto& entity : entities_) {
    entity.loop();
  }

  // Spread virus
  spreadVirus_();

  // Execute next day logic (Must be called last)
  if (currentMinute_ >= config_.MINUTES_IN_A_DAY()) {
    dayLoop_();
  }
}

// Other methods ///////////////////////////////////////////////////////////////
void World::spreadVirus_() {
  // We want to write all tiles with infective entities in here...
  std::set<Coords> infectiveTiles;

  // Loop through every infective entity...
  for (auto& infectiveEntity : entities_) {
    // Dead and not infective entities do not spread the virus.
    if (!infectiveEntity.infective() || infectiveEntity.dead()) {
      continue;
    }

    // If entity succeeds a virusSpreadChance_ test, add tile to infective
    // tiles.
    if (Config::chanceCheck(infectiveEntity.virusSpreadChance())) {
      infectiveTiles.insert(infectiveEntity.pos());
    }
  }

  // Then, loop through every healthy entity and check if they are in an
  // infective tile. (Note the order of the loops: entities->infectiveTiles.
  // It would be less efficient to do the opposite).
  for (auto& healthyEntity : entities_) {
    // Dead and already infected entities cannot get the virus
    if (healthyEntity.infective() || healthyEntity.dead()) {
      continue;
    }

    // If an entity is an infected tile, try to infect it.
    // Note: I considered three algorithms to make this step as efficient
    // as possible : linear std::find in a vector, binary search and
    // set::count(). The latter is the most efficient one in every case.
    // Also note that set::find() is less efficient than set::count().
    // The three only differ significantly for large numbers of infective
    // entities.
    if (infectiveTiles.count(healthyEntity.pos())) {
      healthyEntity.tryInfect();
    }
  }
}

void World::handleQuarantine_(Entity& entity) {
  // Note: this checks are performed on dead entities as well.
  // We could add additional checks to skip dead entities, but it wouldn't
  // benefit us that much, since the performance gains would be
  // insignificant.

  // If entity is infected and not quarantined, put it in quarantine.
  // All this checks are needed to avoid unwanted behaviour (e.g. an entity
  // staying in quarantine forever).
  if (
    entity.infected() && !entity.quarantined()
    && entity.daysSinceLastInfection() > config_.DAYS_AFTER_QUARANTINE()) {
    entity.quarantined(true);
    return;
  }

  // If an entity is  quarantined, check every x days if it can leave
  // quarantine.
  const bool quarantineCheckDay =
    entity.daysSinceLastInfection() % config_.QUARANTINE_CHECK_INTERVAL() == 0;
  if (entity.quarantined() && !entity.infected() && quarantineCheckDay) {
    entity.quarantined(false);
    return;
  }
}

// Getters /////////////////////////////////////////////////////////////////////
int World::currentDay() const noexcept {
  return currentDay_ % config_.DAYS_IN_A_WEEK();
}

int World::currentMinute() const noexcept {
  return currentMinute_;
}

bool World::lockdown() const noexcept {
  return lockdown_;
}

const sf::Image& World::background() {
  return backgroundImage_;
}

const Config& World::config() const noexcept {
  return config_;
}

const Entity& World::entity(int index) const noexcept {
  return entities_[index];
}

const Coords& World::randomParkCoords() {
  if (lockdown()) {
    return invalidCoords_;
  }

  return parkCoords_[Config::randInt(0, parkCoords_.size())];
}

const Coords& World::randomShopCoords() {
  return shopCoords_[Config::randInt(0, shopCoords_.size())];
}

const Coords& World::randomPartyCoords() {
  if (lockdown()) {
    return invalidCoords_;
  }

  return partyCoords_[Config::randInt(0, partyCoords_.size())];
}

bool World::validPosition(const Coords& coords) const noexcept {
  // The only invalid position for now is the one defined in
  // invalidCoords_. (We don't really care if entities end up
  // wandering out of bounds. Can still be expanded later if needed.
  if (coords == invalidCoords_) {
    return false;
  }

  return true;
}

int World::entityCount() const noexcept {
  return entities_.size();
}

int World::infectedCount() const noexcept {
  int infected = 0;

  for (auto& entity : entities_) {
    if (entity.infected() && !entity.dead()) {
      ++infected;
    }
  }

  return infected;
}

int World::deadCount() const noexcept {
  int dead = 0;

  for (auto& entity : entities_) {
    if (entity.dead()) {
      ++dead;
    }
  }

  return dead;
}

int World::immuneCount() const noexcept {
  int immune = 0;

  for (auto& entity : entities_) {
    if (entity.immune()) {
      ++immune;
    }
  }

  return immune;
}

// Setters /////////////////////////////////////////////////////////////////////
void World::lockdown(bool status) noexcept {
  lockdown_ = status;
}

}  // namespace pandemic
