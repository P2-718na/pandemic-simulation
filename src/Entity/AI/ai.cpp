#include "ai.hpp"
#include "entity.hpp"
#include <cstdlib>

//todo check this
// Todo move
int AI::randInt(int min, int max) {
  if (max < min) {
    return AI::randInt(max, min);
  }
  return rand() % (max - min) + min;
}

// Todo move
bool AI::chanceCheck(float chance) {
  return AI::randInt(0, 100) < (int)(100 * chance);
}

void AI::nullAi(Entity* _this, int time) {}

void AI::randomAi(Entity* _this, int time) {
  int x, y;
  x = randInt(100, 700);
  y = randInt(100, 700);

  if (_this->quarantined()) {
    return _this->goHome();
  }

  _this->setDestination(x, y);
}

void AI::testAi(Entity* _this, int time) {
  if (_this->quarantined()) {
    return _this->goHome();
  } else if (time > 7000) {
    return _this->goParty();
  } else if (time > 5000) {
    return _this->goWalk();
  } else if (time > 2000) {
    return _this->goShop();
  }

  int x, y;
  x = randInt(100, 700);
  y = randInt(100, 700);
  _this->setDestination(x, y);
}
