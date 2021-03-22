#include <fstream>
#include <iostream>

#include "seeder.config.h"

using namespace std;

// 1st argument is the number of people to generate
int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Program needs at least one argument!" << endl;

    return 1;
  }

  for (int i = 0; i < atoi(argv[1]); ++i) {
    cout << i << "\n";
  }
}