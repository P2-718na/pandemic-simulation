#include <fstream>
#include <iostream>
#include <string>

#include "seeder.config.h"

using namespace std;

int parsed = 1;

// Arguments
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