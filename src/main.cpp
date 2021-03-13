#include <iostream>
#include "bitmap.hpp"

int main() {
  Bitmap bitmap("test.bmp");

  std::cout << bitmap.height();
}
