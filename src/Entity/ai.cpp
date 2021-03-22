#include "ai.hpp"
#include <cstdlib>

//todo check this
int AI::randInt(int min, int max) {
  if (max < min) {
    return AI::randInt(max, min);
  }
  return rand() % (max - min) + min;
}

void AI::nullAi(Entity* _this, int time) {}

void AI::randomAi(Entity* _this, int time) {
  int x, y;
  x = randInt(100, 700);
  y = randInt(100, 700);

  if (_this->quarantined()) {
    return _this->goHome();
  }

  _this->moveTo(x, y);
}

bool AI::chanceCheck(float chance) {
  return AI::randInt(0, 100) < (int)(100 * chance);
}