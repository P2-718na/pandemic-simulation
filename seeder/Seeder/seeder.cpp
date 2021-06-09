#include <SFML/Graphics/Image.hpp>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "configurations/houses.hpp"
#include "seeder.hpp"

namespace seeder {
// Constructors ////////////////////////////////////////////////////////////////
Seeder::Seeder() = default;
Seeder::Seeder(const std::string& backgroundPath) {
  if (!background_.loadFromFile(backgroundPath)) {
    throw std::runtime_error("Cannot load background_ image.");
  }

  parseBackground_();
  populateHouses_();

  // Shuffle houses. This will make generated entities live in random locations
  // across the map, and not all at the top
  std::shuffle(houses_.begin(), houses_.end(), generator_());
}

// Methods /////////////////////////////////////////////////////////////////////
const pandemic::Coords& Seeder::randomLocation_(
  const std::vector<pandemic::Coords>& list) noexcept {
  return list[randInt(0, list.size())];
}

std::default_random_engine& Seeder::generator_() noexcept {
  static std::random_device rd;
  static std::default_random_engine generator{ rd() };
  return generator;
}

float Seeder::randFloat(float a, float b) noexcept {
  if (b < a) {
    std::swap(a, b);
  }

  std::uniform_real_distribution<float> distrib(a, b);

  return distrib(generator_());
}

int Seeder::randInt(int a, int b) noexcept {
  if (b < a) {
    std::swap(a, b);
  }

  std::uniform_int_distribution<int> distrib(a, b - 1);

  return distrib(generator_());
}

void Seeder::parseBackground_() {
  // Loop through every pixel in image and load POIs into memory
  for (int y = 0; y < background_.getSize().y; ++y) {
    for (int x = 0; x < background_.getSize().x; ++x) {
      sf::Color color = background_.getPixel(x, y);

      if (color == sf::Color(0xff, 0xaa, 0x00)) {
        schoolLocations_.emplace_back(x, y);
        continue;
      }
      if (color == sf::Color(0xff, 0x55, 0x00)) {
        uniLocations_.emplace_back(x, y);
        continue;
      }
      if (color == sf::Color(0xff, 0xff, 0xff)) {
        houseLocations_.emplace_back(x, y);
        continue;
      }
      if (color == sf::Color(0x00, 0x00, 0xcc)) {
        workLocations_.emplace_back(x, y);
        continue;
      }
    }
  }

  if (
    workLocations_.empty() || houseLocations_.empty()
    || schoolLocations_.empty() || uniLocations_.empty()) {
    throw std::runtime_error("Missing locations in image!");
  }
}

void Seeder::populateHouses_() {
  for (auto& houseCoords : houseLocations_) {
    // Generate one house with random type
    switch (randInt(0, 10)) {
      case 0:
        houses_.push_back(FAMILY1());
        break;
      case 1:
        houses_.push_back(FAMILY2());
        break;
      case 2:
        houses_.push_back(FAMILY3());
        break;
      case 3:
        houses_.push_back(COUPLE());
        break;
      case 4:
        houses_.push_back(OLDIES());
        break;
      case 5:
        houses_.push_back(SINGLE());
        break;
      case 6:
        houses_.push_back(CON());
        break;
      case 7:
        houses_.push_back(UNI1());
        break;
      case 8:
        houses_.push_back(UNI2());
        break;
      default:
        houses_.push_back(SINGLE());
    }

    // Assign to each entity of last lastHouse generated its home location
    // and a random work/school/uni location.
    for (auto& inhabitant : houses_.back()) {
      inhabitant.homeLocation = houseCoords;

      // Note that school and uni locations are stored in the same
      // "workLocation" property of an entity, since an entity can't go
      // to school/uni and work at the same time.
      if (inhabitant.age <= 18) {
        inhabitant.workLocation = randomLocation_(schoolLocations_);
        continue;
      }
      if (inhabitant.age <= 25) {
        inhabitant.workLocation = randomLocation_(uniLocations_);
        continue;
      }

      inhabitant.workLocation = randomLocation_(workLocations_);
    }
  }
}

std::string Seeder::makePrintable_(
  const seederEntity& entity, int uid, bool infected) {
  std::stringstream ss{};
  ss << "\n[entity]\n";

  if (infected) {
    ss << "[infected]"
       << "\n";
  }

  ss << "uid=" << uid << "\n";
  ss << "homex=" << entity.homeLocation.first << "\n";
  ss << "homey=" << entity.homeLocation.second << "\n";
  ss << "workx=" << entity.workLocation.first << "\n";
  ss << "worky=" << entity.workLocation.second << "\n";
  ss << "symptoms_resistance=" << entity.symptomsResistance << "\n";
  ss << "virus_spread_chance=" << entity.virusSpreadChance << "\n";
  ss << "infection_resistance=" << entity.infectionResistance << "\n";
  ss << "ai=" << entity.ai << "\n";
  ss << "[/entity]\n";

  return ss.str();
}

void Seeder::generateEntities(int target, int infectedCount) {
  // Reset entity count and printable entities (in case someone wants to call
  // this function more than once).
  entityCount_ = 0;
  printableEntities_ = "";

  // Try to write "target" number of entities
  for (auto& house : houses_) {
    for (auto& inhabitant : house) {
      // Stop once target is reached
      if (entityCount_ == target) {
        // Go to the end of this function, since we still need to add
        // "[count]" keyword at the beginning of the string.
        goto breakAll;
      }

      // add one printable entity to the string. If infected count is greater
      // than zero, set infected to true and decrement it by one.
      printableEntities_ +=
        makePrintable_(inhabitant, entityCount_, infectedCount-- > 0);
      ++entityCount_;
    }
  }

breakAll:
  // Add required [count] keyword at the beginning of the string
  printableEntities_ =
    "[count]\n" + std::to_string(entityCount()) + "\n" + printableEntities_;
}

// Getters /////////////////////////////////////////////////////////////////////
int Seeder::entityCount() const noexcept {
  return entityCount_;
}

const std::string& Seeder::printableEntities() const noexcept {
  return printableEntities_;
}

}  // namespace seeder
