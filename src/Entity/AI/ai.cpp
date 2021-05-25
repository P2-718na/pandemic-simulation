#include "ai.hpp"
#include "entity.hpp"
#include "config.hpp"

//todo check this
// Todo move
void AI::nullAI(Entity* _this, int time) {}

void AI::randomAI(Entity* _this, int time) {
  int x, y;
  x = Config::randInt(100, 700);
  y = Config::randInt(100, 700);

  if (_this->quarantined()) {
    return _this->goHome();
  }

  _this->setDestination(x, y);
}

void AI::testAI(Entity* _this, int time) {
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
  x = Config::randInt(100, 700);
  y = Config::randInt(100, 700);
  _this->setDestination(x, y);
}
