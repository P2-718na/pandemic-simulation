#include "entity.hpp"

struct AI {
  static int randInt(int min, int max);
  static void nullAi(Entity* _this);
  static void randomAi(Entity* _this);
};