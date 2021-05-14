#include "world.hpp"

#include <fstream>
#include <iostream>

#include "Entity/AI/ai.hpp"
#include "entity.hpp"

// Private methods /////////////////////////////////////////////////////////////
void World::initMap_() {
  this->map_ =
    Map(
      this->width_,
      Column(this->height_, {0})
    );
}

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
  this->initMap_();
  World::parseEntitiesFromFile_(entitiesFile, this->entities);
  this->parseBackground_();
}

// Accessors ///////////////////////////////////////////////////////////////////
int World::time() const {
  return this->currentMinute_;
}

sf::Image World::background() {
  return this->backgroundImage_;
}

// FIXME these three need to be optimized
std::pair<int, int> World::randomParkCoords() {
  return this->parkCoords_[AI::randInt(0, this->parkCoords_.size())];
}
std::pair<int, int> World::randomShopCoords() {
  return this->shopCoords_[AI::randInt(0, this->shopCoords_.size())];
}
std::pair<int, int> World::randomPartyCoords() {
  return this->partyCoords_[AI::randInt(0, this->partyCoords_.size())];
}

// Methods /////////////////////////////////////////////////////////////////////
bool World::isInside(const Entity &entity) const {
  const int posX = entity.posX();
  const int posY = entity.posY();

  return posX < this->width_ && posX >= 0 && posY < this->height_ && posY >= 0;
}

day World::weekDay() const {
  return (day)(this->currentDay_ % 7);
}

bool World::parseEntitiesFromFile_(
  const std::string &entitiesFile, std::vector<Entity> &entities) {
  if (!entities.empty()) {
    return false;
  }

  int uid, homex, homey, workx, worky;
  float virus_resistance, virus_spread_chance, infection_chance;
  std::string ai;

  std::ifstream ifs(entitiesFile);
  std::string line;

  int count = 0;

  while(std::getline(ifs, line)) {
    if (line == "[count]") {
      if (count != 0) {
        return false;
      }

      ifs >> count;
      entities.reserve(count);
      continue;
    }

    if (line == "[entity]") {
      entities.emplace_back((IWorld*)this, uid, homex, homey, parseEntityAi(ai));

      Entity& entity = entities.back();
      entity.homeLocation = {homex, homey};
      entity.workLocation = {workx, worky};
      entity.symptomsResistance = virus_resistance;
      entity.virusSpreadChance = virus_spread_chance;
      entity.infectionResistance = infection_chance;

      continue;
    }

    if (line == "[infected]") {
      entities.back().infective(true);
      continue;
    }

    auto delim = line.find('=');
    if (delim == std::string::npos) {
      continue;
    }

    std::string key = line.substr(0, delim);
    std::string value = line.substr(delim + 1, std::string::npos);
    Entity &currentEntity = entities[entities.size() - 1];

    if (key == "uid") {
      uid = atoi(value.c_str());
      continue;
    }
    if (key == "homex") {
      homex = atoi(value.c_str());
      continue;
    }
    if (key == "homey") {
      homey = atoi(value.c_str());
      continue;
    }
    if (key == "workx") {
      workx = atoi(value.c_str());
      continue;
    }
    if (key == "worky") {
      worky = atoi(value.c_str());
      continue;
    }
    if (key == "virus_resistance") {
      virus_resistance = atof(value.c_str());
      continue;
    }
    if (key == "virus_spread_chance") {
      virus_spread_chance = atof(value.c_str());
      continue;
    }
    if (key == "infection_chance") {
      infection_chance = atof(value.c_str());
      continue;
    }
    if (key == "ai") {
      ai = value;
      continue;
    }
  }
  return true;
}

entityAi World::parseEntityAi(const std::string &value) {
  if (value == "nullAi") {
    return AI::nullAi;
  }
  if (value == "randomAi") {
    return AI::randomAi;
  }
  if (value == "testAi") {
    return AI::testAi;
  }

  return AI::testAi;
}
