#include <iostream>
#include "bitmap.hpp"
#include "pathfinder.hpp"

int main() {
  Bitmap bitmap("test.bmp");
  std::cout << "Width:" << bitmap.width() << "Height:" << bitmap.height() << std::endl;

  std::cout << bitmap.at(9, 8) << " ";

  Pathfinder pathfinder(0, 0, 6, 9);

  int* pos;
  for (int i = 0; i < 10; i++) {
    pos = pathfinder.step();

    std::cout << pos[0] << " " << pos[1] << std::endl;
  }
}
