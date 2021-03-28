#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <SFML/Graphics/Image.hpp>

#include "seeder.config.h"

using namespace std;

// Globals
sf::Image background;
ofstream ofs;

vector<pair<int, int>> houseLocations;
vector<pair<int, int>> workLocations;
vector<pair<int, int>> schoolLocations;
vector<pair<int, int>> uniLocations;

// This is used for internal generation
vector<House> houses;

// Arguments
int parsed = 1;
int target = 0;
string mapFile = "";
string outFile = "entities";

void help() {
  char help[] =
    "Seeder for entities.\n"
    "Commands:\n"
    "--help         Show this message.\n"
    "\n"
    "--target       Target number of entities to generate (required).\n"
    "               Use -1 for unlimited entities.\n"
    "\n"
    "--map          Map file to read entities from (required).\n"
    "\n"
    "--output       Output file (default: ./entities).\n";

  cout << help;
}

void parseArg(int argc, char* argv[]) {
  if (strcmp(argv[parsed], "--help") == 0) {
    help();
    exit(0);
  }
  if (strcmp(argv[parsed], "--target") == 0) {
    ++parsed;

    if (parsed == argc) {
      throw runtime_error("Invalid parameter count");
    }
    target = atoi(argv[parsed]);

    if (target < -1) {
      throw runtime_error("Invalid target");
    }

    cout << "Target: " << target << endl;
    ++parsed;

    return;
  }
  if (strcmp(argv[parsed], "--map") == 0) {
    ++parsed;

    if (parsed == argc) {
      throw runtime_error("Invalid parameter count");
    }
    mapFile = argv[parsed];
    cout << "Map file: " << mapFile << endl;
    ++parsed;

    return;
  }
  if (strcmp(argv[parsed], "--output") == 0) {
    ++parsed;

    if (parsed == argc) {
      throw runtime_error("Invalid parameter count");
    }
    outFile = argv[parsed];
    cout << "Output file: " << outFile << endl;
    ++parsed;

    return;
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
}

void populateHouses() {
  for (auto &house : houseLocations) {
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

    // todo write work locations;

    houses.back().posx = house.first;
    houses.back().posy = house.second;
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

void writeEntity(const Entity &entity, int uid) {
  // (See entity format in entities.sample.txt)
  ofs << "[entity]\n";
  ofs << "uid=" << uid << "\n";
  ofs << "homex=" << entity.homeLocation.first << "\n";
  ofs << "homey=" << entity.homeLocation.second << "\n";
  ofs << "workx=" << entity.workLocation.first << "\n";
  ofs << "worky=" << entity.workLocation.second << "\n";
  ofs << "virus_resistance=" << entity.virusResistance << "\n";
  ofs << "virus_spread_chance=" << entity.virusSpreadChance << "\n";
  ofs << "infection_chance=" << entity.infectionChance << "\n";
  ofs << "ai=" << entity.ai << "\n";
  ofs << "\n";
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

      // Set home location based on current house.
      // This could be moved elsewhere
      inhab.homeLocation.first = house.posx;
      inhab.homeLocation.second = house.posy;

      // Write entity to file
      writeEntity(inhab, target_);
      --target_;
    }
  }
}

int main(int argc, char* argv[]) {
  // Parse arguments
  try {
    // If no arguments provided, print help
    if (argc < 2) {
      help();
      return 0;
    }

    // Parse each argument
    while (parsed < argc) {
      parseArg(argc, argv);
    }

    // MapFile is required. Check and open it
    if (mapFile.empty()) {
      throw runtime_error("Must specify input map file");
    }
    if (!background.loadFromFile(mapFile)) {
      throw runtime_error("Invalid map file");
    };

    // Open output file. Throws on fail.
    ofs.open(outFile);
  } catch (runtime_error &err) {
    cerr << "Error initialising: " << err.what() << endl;

    exit(1);
  }

  // Load POI locations from image into memory
  parseImage();

  // Reserve space for houses (Not needed, increases performance a bit)
  houses.reserve(houseLocations.size());

  // Insert random families into available houses
  populateHouses();

  // Print info
  ofs << "[count]" << endl;
  ofs << (target > 0 ? target : countEntities()) << endl;
  ofs << endl;

  // Write entities to file until target is reached
  writeEntitiesUntilTarget();

  // Save data to file
  ofs.close();

  // Print result
  cout << "Finish. Wrote ";
  cout << (target > 0 ? target : countEntities());
  cout << " entities" << endl;
}