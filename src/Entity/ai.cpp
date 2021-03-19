#include "ai.hpp"

//todo check this
int randint (int min, int max) {
  return rand() % (max - min) + min;
}

void nullAi(Entity* _this) {}

void randomAi(Entity* _this) {
  int x, y;
  x = randint(100, 700);
  y = randint(100, 700);

  _this->moveTo(x, y);
}