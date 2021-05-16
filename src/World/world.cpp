#include "world.hpp"

#include <fstream>
#include <iostream>

#include "Entity/AI/ai.hpp"
#include "entity.hpp"

// Private methods /////////////////////////////////////////////////////////////
void World::parseBackground_() {
  for (int y = 0; y < this->height_; ++y) {
    for (int x = 0; x < this->width_; ++x ) {
      sf::Color color = this->backgroundImage_.getPixel(x, y);

      // Todo write these values to config
      // Parse image to find walk, shop and party POIs
      if (color.r == 0x00 && color.g == 0xff && color.b == 0x00) {
        this->parkCoords_.emplace_back(x, y);
      } else if (color.r == 0xff && color.g == 0xFF && color.b == 0x00) {
        this->shopCoords_.emplace_back(x, y);
      } else if (color.r == 0xff && color.g == 0x00 && color.b == 0xff) {
        this->partyCoords_.emplace_back(x, y);
      }
    }
  }
}

// Constructors ////////////////////////////////////////////////////////////////
World::World(
  const std::string &backgroundImagePath,
  const std::string &entitiesFile
) {
  // fixme cleanup these two constructors
  if (!this->backgroundImage_.loadFromFile(backgroundImagePath)) {
    throw std::runtime_error("Cannot load image");
  };

  this->width_ = this->backgroundImage_.getSize().x;
  this->height_ = this->backgroundImage_.getSize().y;
  World::parseEntitiesFromFile_(entitiesFile, this->entities);
  this->parseBackground_();
}

// Accessors ///////////////////////////////////////////////////////////////////
Day World::currentDay() const {
  return static_cast<Day>((currentDay_ % 7));
}

int World::currentMinute() const {
  return this->currentMinute_;
}

const sf::Image& World::background() {
  return backgroundImage_;
}

const Coords& World::randomParkCoords() {
  return parkCoords_[AI::randInt(0, parkCoords_.size())];
}

const Coords& World::randomShopCoords() {
  return shopCoords_[AI::randInt(0, shopCoords_.size())];
}

const Coords& World::randomPartyCoords() {
  return partyCoords_[AI::randInt(0, partyCoords_.size())];
}

int World::infectedCount() const {
  int infected = 0;

  for (auto & entity : this->entities) {
    if (entity.infected()) {
      ++infected;
    }
  }

  return infected;
}

int World::deadCount() const {
  int dead = 0;

  for (auto & entity : this->entities) {
    if (entity.dead()) {
      ++dead;
    }
  }

  return dead;
}
