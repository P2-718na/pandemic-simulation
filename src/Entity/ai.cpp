#include "ai.hpp"

//todo check this
int AI::randInt(int min, int max) {
  if (max < min) {
    return AI::randInt(max, min);
  }
  return rand() % (max - min) + min;
}

void AI::nullAi(Entity* _this) {}

void AI::randomAi(Entity* _this) {
  int x, y;
  x = randInt(100, 700);
  y = randInt(100, 700);

  _this->moveTo(x, y);
}