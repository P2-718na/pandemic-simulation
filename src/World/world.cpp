#include "world.hpp"

#include <algorithm>
#include <stdexcept>

#include "config.hpp"
#include "entity.hpp"
#include "parser.hpp"

// Loops ///////////////////////////////////////////////////////////////////////
void World::dayLoop_() {
  // Increment day counter
  ++currentDay_;
  // and reset minute counter.
  currentMinute_ = 0;

  // For every entity, run its dayLoop and handle quarantine logic.
  for (auto &entity : entities_) {
    entity.dayLoop();
    handleQuarantine_(entity);
  }

  // Todo move this elsewhere
  printf("New day! %d\nInfected: %d, Dead: %d\n", currentDay_, infectedCount(),
    deadCount());
}

void World::loop() {
  // Increment minute counter
  ++currentMinute_;

  // Loop every entity
  for (auto &entity : entities_) {
    entity.loop();
  }

  // Spread virus
  spreadVirus_();

  // Execute next day logic (Must be called last)
  if (currentMinute_ >= config_.MINUTES_IN_A_DAY) {
    dayLoop_();
  }
}

// Other methods ///////////////////////////////////////////////////////////////
void World::spreadVirus_() {
  // We want to write all tiles with infective entities in here...
  std::vector<Coords> infectiveTiles;

  // Loop through every infective entity...
  for (auto &infectiveEntity : entities_) {
    if (!infectiveEntity.infective()) {
      continue;
    }

    // If entity succeeds a virusSpreadChance test, add tile to infective
    // tiles.
    if (AI::chanceCheck(infectiveEntity.virusSpreadChance)) {
      infectiveTiles.push_back(infectiveEntity.pos());
    }
  }

  // Then, loop through every entity and check if they are in an infective
  // tile. (Note the order of the loops: entities->infectiveTiles. It would
  // be less efficient to do the opposite).
  for (auto &entity : entities_) {
    // todo this could probably be optimized using a set.
    const auto &first = infectiveTiles.begin();
    const auto &last = infectiveTiles.end();

    // Check if current entity is in an infected tile.
    const bool isInInfectedTile = std::find(first, last, entity.pos()) != last;

    // If it is, try to infect it.
    if (isInInfectedTile) {
      entity.tryInfect();
    }
  }
}

void World::handleQuarantine_(Entity &entity) {
  // Note: this checks are performed on dead entities as well.
  // We could add additional checks to skip dead entities, but it wouldn't
  // benefit us that much, since the performance gains would be
  // insignificant.

  // If entity is infected and not quarantined, put it in quarantine.
  // All this checks are needed to avoid unwanted behaviour (e.g. an entity
  // staying in quarantine forever).
  if (entity.infected() && !entity.quarantined &&
      entity.daysSinceLastInfection() > config_.DAYS_AFTER_QUARANTINE) {
    entity.quarantined = true;
    return;
  }

  // If an entity is  quarantined, check every x days if it can leave
  // quarantine.
  const bool quarantineCheckDay =
    entity.daysSinceLastInfection() % config_.QUARANTINE_CHECK_INTERVAL == 0;
  if (entity.quarantined && !entity.infected() && quarantineCheckDay) {
    entity.quarantined = false;
    return;
  }
}

// Constructors ////////////////////////////////////////////////////////////////
World::World(const std::string &backgroundImagePath,
  const std::string &entitiesFile, Config &config)
  : config_{config} {
  // Load background image
  if (!backgroundImage_.loadFromFile(backgroundImagePath)) {
    throw std::runtime_error("Cannot load image from file.");
  }

  // Load entities
  if (!Parser::parseEntitiesFile(this, entitiesFile, entities_)) {
    throw std::runtime_error("Error parsing entities file.");
  }

  // Load points of interest.
  if (!Parser::parsePointsOfInterests(
        config, backgroundImage_, parkCoords_, shopCoords_, partyCoords_)) {
    throw std::runtime_error("Error parsing points of interest for entities.");
  }

  // Initialize Pathfinder using MicroPather.
  map_.init(config_, backgroundImage_);
  Pathfinder::setMap(&map_);
}

// Accessors ///////////////////////////////////////////////////////////////////
int World::currentDay() const noexcept {
  return currentDay_ % config_.DAYS_IN_A_WEEK;
}

int World::currentMinute() const noexcept {
  return this->currentMinute_;
}

const sf::Image &World::background() {
  return backgroundImage_;
}

const Config &World::config() const noexcept {
  return config_;
}

const std::vector<Entity> &World::entities() const noexcept {
  return entities_;
}

const Coords &World::randomParkCoords() {
  return parkCoords_[AI::randInt(0, parkCoords_.size())];
}

const Coords &World::randomShopCoords() {
  return shopCoords_[AI::randInt(0, shopCoords_.size())];
}

const Coords &World::randomPartyCoords() {
  return partyCoords_[AI::randInt(0, partyCoords_.size())];
}

int World::infectedCount() const noexcept {
  int infected = 0;

  for (auto &entity : this->entities_) {
    if (entity.infected()) {
      ++infected;
    }
  }

  return infected;
}

int World::deadCount() const noexcept {
  int dead = 0;

  for (auto &entity : this->entities_) {
    if (entity.dead()) {
      ++dead;
    }
  }

  return dead;
}
