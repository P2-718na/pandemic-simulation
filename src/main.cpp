#include <iostream>
#include "bitmap.hpp"

int main() {
  Bitmap bitmap("test.bmp");
  std::cout << "Width:" << bitmap.width() << "Height:" << bitmap.height() << std::endl;

  std::cout << bitmap.at(9, 8) << " ";
}
