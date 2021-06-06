// todo refactor this

#include <random>
#include <string>
#include <vector>
#include <SFML/Graphics/Image.hpp>

#include "seeder.hpp"
#include "configurations/houses.hpp"

namespace seeder {
// Constructors ////////////////////////////////////////////////////////////////
Seeder::Seeder() noexcept = default;

Seeder::Seeder(const std::string& backgroundPath) {
  //todo
}

// Methods /////////////////////////////////////////////////////////////////////
const pandemic::Coords& Seeder::randomLocation_(
  const std::vector<pandemic::Coords>& list) noexcept {
  return list[randInt_(0, list.size())];
}

std::default_random_engine& Seeder::generator_() noexcept {
  static std::random_device rd;
  static std::default_random_engine generator{rd()};
  return generator;
}

float Seeder::randFloat_(float a, float b) noexcept {
  if (b > a) {
    std::swap(a, b);
  }

  std::uniform_real_distribution<float> distrib(a, b);

  return distrib(generator_());
}

float Seeder::randInt_(int a, int b) noexcept {
  if (b > a) {
    std::swap(a, b);
  }

  std::uniform_int_distribution<int> distrib(a, b - 1);

  return distrib(generator_());
}

}

void parseImage() {
  // Loop through every pixel in image
  for (int y = 0; y < background.getSize().y; ++y) {
    for (int x = 0; x < background.getSize().x; ++x ) {
      sf::Color color = background.getPixel(x, y);

      if (color.r == 0xff && color.g == 0xaa && color.b == 0x00) {
        schoolLocations.emplace_back(x, y);
        continue;
      }
      if (color.r == 0xff && color.g == 0x55 && color.b == 0x00) {
        uniLocations.emplace_back(x, y);
        continue;
      }
      if (color.r == 0xff && color.g == 0xff && color.b == 0xff) {
        houseLocations.emplace_back(x, y);
        continue;
      }
      if (color.r == 0x00 && color.g == 0x00 && color.b == 0xcc) {
        workLocations.emplace_back(x, y);
        continue;
      }
    }
  }

  if (workLocations.empty() || houseLocations.empty() || schoolLocations.empty() || uniLocations.empty()) {
    throw std::runtime_error("Missing locations in image!");
  }
}

void populateHouses() {
  for (auto &houseCoords : houseLocations) {
    switch ((int)randFloat(0,  9)) {
      case 0:
        houses.push_back(FAMILY1());
        break;
      case 1:
        houses.push_back(FAMILY2());
        break;
      case 2:
        houses.push_back(FAMILY3());
        break;
      case 3:
        houses.push_back(COUPLE());
        break;
      case 4:
        houses.push_back(OLDIES());
        break;
      case 5:
        houses.push_back(SINGLE());
        break;
      case 6:
        houses.push_back(CON());
        break;
      case 7:
        houses.push_back(UNI1());
        break;
      case 8:
        houses.push_back(UNI2());
        break;
      default:
        houses.push_back(SINGLE());
    }

    House &house = houses.back();

    // Set POI coords for house inhabitants
    for (auto &inhab : house.inhabs) {
      inhab.homeLocation = houseCoords;

      // No work location needed for elder people
      // and younger than 12 years
      if (inhab.age <= 18) {
        inhab.workLocation = randomLocation(schoolLocations);
        continue;
      }
      if (inhab.age <= 25) {
        inhab.workLocation = randomLocation(uniLocations);
        continue;
      }

      inhab.workLocation = randomLocation(workLocations);
    }
  }
}

int countEntities() {
  int count = 0;
  for (auto &house : houses) {
    for (auto &inhab : house.inhabs) {
      ++count;
    }
  }

  return count;
}


/*
void writeEntity(const seederEntity &entity, int uid) {
  // (See entity format in entities.sample.txt)
  ofs << "\n";
  ofs << "uid=" << uid << "\n";
  ofs << "homex=" << entity.homeLocation.first << "\n";
  ofs << "homey=" << entity.homeLocation.second << "\n";
  ofs << "workx=" << entity.workLocation.first << "\n";
  ofs << "worky=" << entity.workLocation.second << "\n";
  ofs << "virus_resistance=" << entity.symptomsResistance << "\n";
  ofs << "virus_spread_chance=" << entity.virusSpreadChance << "\n";
  ofs << "infection_chance=" << entity.infectionResistance << "\n";
  ofs << "ai=" << entity.ai << "\n";
  ofs << "[entity]\n";
}

void writeEntitiesUntilTarget() {
  // Do not modify original target since we want to print it later
  int target_ = target;

  for (auto &house : houses) {
    for (auto &inhab : house.inhabs) {
      // Stop once target is reached
      if (target_ == 0) {
        return;
      }

      // Write entity to file
      writeEntity(inhab, target_);
      --target_;
    }
  }
}
*/
