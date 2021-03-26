#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <SFML/Graphics/Image.hpp>

#include "seeder.config.h"

using namespace std;

enum Sex {
  male,
  female
};

// Entity
struct Entity {
  int age;
  Sex sex;

};

// Globals
sf::Image background;
vector<pair<int, int>> homeLocations;
vector<pair<int, int>> workLocations;
vector<pair<int, int>> schoolLocations;

// Arguments
int parsed = 1;
int genTarget = 0;
string mapFile = "";
string outFile = "entities";

void help() {
  char help[] =
    "Seeder for entities.\n"
    "Commands:\n"
    "--help         show this message\n"
    "--target       target number of entities to generate (required)\n"
    "--map          map file to read entities from (required)\n"
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
    genTarget = atoi(argv[parsed]);
    cout << "Target: " << genTarget << endl;
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
      } else if (color.r == 0xff && color.g == 0xff && color.b == 0xff) {
        homeLocations.emplace_back(x, y);
      } else if (color.r == 0x0 && color.g == 0x0 && color.b == 0xcc) {
        workLocations.emplace_back(x, y);
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

  ofs << "[count]" << endl << genTarget << endl << endl;

  background.loadFromFile(mapFile);
  parseImage();



  while (genTarget --> 0) {
    ofs <<
      "[entity]\n"
      "uid=" << genTarget << "\n"
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