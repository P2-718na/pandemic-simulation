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

vector<pair<int, int>> homeLocations;
vector<pair<int, int>> workLocations;
vector<pair<int, int>> schoolLocations;
vector<pair<int, int>> uniLocations;

// This is used for internal generation
vector<House> houses;

vector<Entity> entities;

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
  for (int y = 0; y < background.getSize().y; ++y) {
    for (int x = 0; x < background.getSize().x; ++x ) {
      sf::Color color = background.getPixel(x, y);
      if (color.r == 0xff && color.g == 0xaa && color.b == 0x0) {
        schoolLocations.emplace_back(x, y);
        continue;
      }
      if (color.r == 0xff && color.g == 0x55 && color.b == 0x0) {
        uniLocations.emplace_back(x, y);
        continue;
      }
      if (color.r == 0xff && color.g == 0xff && color.b == 0xff) {
        homeLocations.emplace_back(x, y);
        continue;
      }
      if (color.r == 0x0 && color.g == 0x0 && color.b == 0xcc) {
        workLocations.emplace_back(x, y);
        continue;
      }
    }
  }
}

void printEntity(const Entity &entity, int uid) {
  ofs << "[entity]\n";
  ofs << "uid=" << uid << "\n";
  ofs << "homex=" << entity.homeLocation.first << "\n";
  ofs << "homex=" << entity.homeLocation.second << "\n";
  ofs << "workx=" << entity.workLocation.first << "\n";
  ofs << "worky=" << entity.workLocation.second << "\n";
  ofs << "virus_resistance=" << entity.virusResistance << "\n";
  ofs << "virus_spread_chance=" << entity.virusSpreadChance << "\n";
  ofs << "infection_chance=" << entity.infectionChance << "\n";
  ofs << "ai=" << entity.ai << "\n";
  ofs << "\n";
}

void writeEntitiesUntilTarget() {
  for (auto &house : houses) {
    for (auto &inhab : house.inhabs) {
      if (target == 0) {
        return;
      }

      inhab.homeLocation.first = house.posx;
      inhab.homeLocation.second = house.posy;

      printEntity(inhab, target);
      --target;
    }
  }
}

int main(int argc, char* argv[]) {
  // Parse arguments
  try {
    if (argc < 2) {
      help();
      return 0;
    }

    while (parsed < argc) {
      parseArg(argc, argv);
    }

    if (mapFile.empty()) {
      throw runtime_error("Must specify input map file");
    }

    ofs.open(outFile);
  } catch (runtime_error &err) {
    cerr << "Error initialising: " << err.what() << endl;
    exit(1);
  }

  // Todo check for -1
  ofs << "[count]" << endl << target << endl << endl;

  background.loadFromFile(mapFile);
  parseImage();

  houses.reserve(homeLocations.size());

  for (auto &home : homeLocations) {
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

    // todo write work and home locations;

    houses.back().posx = home.first;
    houses.back().posy = home.second;
  }

  cout <<
       "Finish. Wrote " <<
       (target > 0 ? target : - target - 1)  <<
       " entities";

  writeEntitiesUntilTarget();

  ofs.close();
  cout << endl;
}