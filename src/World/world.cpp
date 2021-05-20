#include "world.hpp"

#include <fstream>
#include <iostream>

#include "Entity/AI/ai.hpp"
#include "entity.hpp"

// Private methods /////////////////////////////////////////////////////////////
void World::spreadVirus_() {
  std::vector<Coords> infectiveTiles;

  // Save coordinates for each infective entity...
  for (auto &infectiveEntity : this->entities_) {
    if (!infectiveEntity.infective()) {
      continue;
    }

    // An infective entity has to succeed a virus spread test
    if (AI::chanceCheck(infectiveEntity.virusSpreadChance)) {
      infectiveTiles.emplace_back(
        infectiveEntity.posX(), infectiveEntity.posY());
    }
  }

  // And spread virus
  for (auto &entity : this->entities_) {
    // Map is not very efficient compared to this
    const auto &first = infectiveTiles.begin();
    const auto &last = infectiveTiles.end();
    const bool contactWithInfected =
      std::find(first, last, entity.pos()) != last;
    if (contactWithInfected) {
      entity.tryInfect();
    }
  }
}

void World::handleQuarantine_(Entity &entity) {
  // Put person in quarantine after virus gets diagnosed.
  if (entity.infected() &&
      !entity.quarantined &&
      entity.daysSinceLastInfection() > this->config_.DAYS_AFTER_QUARANTINE) {
    entity.quarantined = true;
    return;
  }

  // if quarantined, check every x days if it can leave quarantine.
  const bool quarantineCheckDay =
    entity.daysSinceLastInfection() % this->config_.QUARANTINE_CHECK_INTERVAL ==
    0;
  if (entity.quarantined && !entity.infected() && quarantineCheckDay) {
    entity.quarantined = false;
    return;
  }
}

// Constructors ////////////////////////////////////////////////////////////////
World::World(const std::string &backgroundImagePath,
  const std::string &entitiesFile, Config &config)
  : config_{config} {
  // fixme cleanup these two constructors
  if (!this->backgroundImage_.loadFromFile(backgroundImagePath)) {
    throw std::runtime_error("Cannot load image");
  };

  this->width_ = this->backgroundImage_.getSize().x;
  this->height_ = this->backgroundImage_.getSize().y;
  World::loadEntitiesFromFile_(entitiesFile, this->entities_);
  this->parseBackground_();
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

const Config& World::config() const noexcept {
  return config_;
}

const std::vector<Entity>& World::entities() const noexcept {
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
