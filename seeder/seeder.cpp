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
vector<pair<int, int>> homeLocations;
vector<pair<int, int>> workLocations;
vector<pair<int, int>> schoolLocations;
vector<pair<int, int>> uniLocations;
vector<Entity> entities;
vector<vector<Entity>> houses;

// Arguments
int parsed = 1;
int target = 0;
string mapFile = "";
string outFile = "entities";

void help() {
  char help[] =
    "Seeder for entities.\n"
    "Commands:\n"
    "--help         show this message\nù"
    "\n"
    "--target       target number of entities to generate (required)\n"
    "               Use -1 for unlimited entities\n"
    "\n"
    "--map          map file to read entities from (required)\n"
    "\n"
    "--output       output file (default: ./entities)\n";

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

// 1st argument is the number of people to generate
int main(int argc, char* argv[]) {
  ofstream ofs;
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
  }

  ofs << "[count]" << endl << target << endl << endl;

  background.loadFromFile(mapFile);
  parseImage();

  if (target != -1) {
    entities.reserve(target);
  }
  houses.reserve(homeLocations.size());



  while (target --> 0) {
    ofs <<
      "[entity]\n"
      "uid=" << target
        << "\n"
      "homex=0\n"
      "homey=0\n"
      "workx=0\n"
      "worky=0\n"
      "virus_resistance=.9\n"
      "virus_spread_chance=.5\n"
      "infection_chance=.8\n"
      "ai=randomAi\n\n";
  }
}