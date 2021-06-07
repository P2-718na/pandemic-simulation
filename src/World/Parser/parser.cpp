#include <fstream>
#include <cassert>
#include <bitset>

#include "parser.hpp"
#include "config.hpp"
#include "entity.hpp"

namespace pandemic {

void World::Parser::parsePointsOfInterests(const Config& config,
  const sf::Image& backgroundImage, std::vector<Coords>& parkCoords,
  std::vector<Coords>& shopCoords, std::vector<Coords>& partyCoords) {
  const int height = backgroundImage.getSize().x;
  const int width = backgroundImage.getSize().y;

  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {
      sf::Color color = backgroundImage.getPixel(row, column);

      if (color == config.PARK_COLOR()) {
        parkCoords.emplace_back(row, column);
        continue;
      }
      if (color == config.SHOP_COLOR()) {
        shopCoords.emplace_back(row, column);
        continue;
      }
      if (color == config.PARTY_COLOR()) {
        partyCoords.emplace_back(row, column);
        continue;
      }
    }
  }

  // We want at least one POI in every list.
  if (parkCoords.empty() || shopCoords.empty() || partyCoords.empty()) {
    throw std::runtime_error("Missing points of interest in background image!");
  }
}

void World::Parser::parseEntitiesFile(
  World* parentPtr, const std::string& entitiesFile,
  std::vector<Entity>& entities) {
  if (!entities.empty()) {
    // Entities vector must be empty, since we must reserve all the needed
    // capacity at the beginning.
    throw std::runtime_error("Trying to initialize a non empty entity vector!");
  }

  std::ifstream is(entitiesFile);
  std::string line;

  // This is needed to check that the number of parsed entities corresponds to
  // [count] specified at the beginning, and to check that [count] keyword
  // appears before parsing any entities (See comment below).
  int parsedEntities{0};
  int count;

  while (std::getline(is, line)) {
    // "[count]" keyword must appear before any entity. This is necessary for
    // the program to work correctly (See comment below)
    if (line == "[count]") {
      if (parsedEntities != 0) {
        throw std::runtime_error("[count] keyword missing");
      }
        is >> count;

        // NOTE: This step is needed for the correct functioning of the program.
        // Every entity contains an AI object, which takes a pointer to the
        // entity itself. If we resize the vector mid initialization, all the
        // pointers will be rendered invalid and break everything.
        entities.reserve(count);
        continue;
    }

    // Every entity begins with [/entity] tag. If we are not parsing an entity,
    // everything else is discarded.
    if (line == "[entity]") {
      // These are all the values we need to create a new entity.
      int uid{};
      Coords homeCoords{};
      Coords workCoords{};
      float virus_resistance{};
      float virus_spread_chance{};
      float infection_chance{};
      std::string ai{};
      bool infective{false};

      // Store which values have been read for current entity. This will be
      // Used later to check that every required value has been read.
      // 0b 0 0 0 0 0 0 0 0 0
      //    8 7 6 5 4 3 2 1 0  <-- position in bitset
      // 0 = uid                   (required)
      // 1 = homex                 (required)
      // 2 = homey                 (required)
      // 3 = workx                 (required)
      // 4 = worky                 (required)
      // 5 = virus_resistance
      // 6 = virus_spread_chance
      // 7 = infection_chance
      // 8 = ai
      std::bitset<9> parsedValues(0b0);

      // Read all values one by one and store them in memory
      while (std::getline(is, line)) {
        // Every entity ends with [/entity] tag. If this is the last line for
        // this entity, exit this while loop.
        if (line == "[/entity]") {
          break;
        }

        // If entity is infected, set it to true and go to next line.
        if (line == "[infected]") {
          // NOTE: we set infective and not infected to true, since there would
          // be no real difference in the simulation. Setting infected to true
          // would mean that we would need to wait three days before seeing
          // anything interesting.
          infective = true;
          continue;
        }

        // Find "=" character in current line.
        auto delim = line.find('=');
        if (delim == std::string::npos) {
          // If the character was not found, skip this line.
          continue;
        }

        // "key" is everything before "="
        std::string key = line.substr(0, delim);
        // "value" is everything after "="
        std::string value = line.substr(delim + 1, std::string::npos);

        if (key == "uid") {
          uid = atoi(value.c_str());
          parsedValues.flip(0);
          continue;
        }
        if (key == "homex") {
          homeCoords.first = atoi(value.c_str());
          parsedValues.flip(1);
          continue;
        }
        if (key == "homey") {
          homeCoords.second = atoi(value.c_str());
          parsedValues.flip(2);
          continue;
        }
        if (key == "workx") {
          workCoords.first = atoi(value.c_str());
          parsedValues.flip(3);
          continue;
        }
        if (key == "worky") {
          workCoords.second = atoi(value.c_str());
          parsedValues.flip(4);
          continue;
        }
        if (key == "virus_resistance") {
          virus_resistance = atof(value.c_str());
          parsedValues.flip(5);
          continue;
        }
        if (key == "virus_spread_chance") {
          virus_spread_chance = atof(value.c_str());
          parsedValues.flip(6);
          continue;
        }
        if (key == "infection_chance") {
          infection_chance = atof(value.c_str());
          parsedValues.flip(7);
          continue;
        }
        if (key == "ai") {
          ai = value;
          parsedValues.flip(8);
          continue;
        }
      }

      // Every entity needs to have all mandatory values specified.
      // (See comments above for bitmap meaning)
      const std::bitset<9> requiredBits(0b000011111);
      if ((parsedValues & requiredBits) != requiredBits) {
        throw std::runtime_error("Tried to parse invalid entity.");
      }

      // Do some value checking to avoid unexpected behaviour
      if (abs(homeCoords.first) > 10000 || abs(homeCoords.second) > 10000) {
        throw std::range_error("Entity home coordinates are too big!");
      }
      if (abs(workCoords.first) > 10000 || abs(workCoords.second) > 10000) {
        throw std::range_error("Entity work coordinates are too big!");
      }

      // Create new entity and set all specified values.
      // If AI is set, use it. Otherwise use default.
      if (parsedValues.test(8)) {
        entities.emplace_back(parentPtr, uid, homeCoords.first, homeCoords.second, ai);
      } else {
        entities.emplace_back(parentPtr, uid, homeCoords.first, homeCoords.second);
      }

      // Keep track of how many entities have been parsed.
      ++parsedEntities;

      // Mandatory values
      Entity& currentEntity = entities.back();
      currentEntity.homeLocation(homeCoords);
      currentEntity.workLocation(workCoords);
      currentEntity.infective(infective);

      // Optional values
      // todo change names to reflect Entity

      if (parsedValues.test(5)) {
        currentEntity.symptomsResistance(virus_resistance);
      }
      if (parsedValues.test(6)) {
        currentEntity.virusSpreadChance(virus_spread_chance);
      }
      if (parsedValues.test(7)) {
        currentEntity.infectionResistance(infection_chance);
      }
    }
  }

  if (parsedEntities != count) {
    throw std::runtime_error("Invalid [count] keyword in entities file!");
  }
}

}  // namespace pandemic
