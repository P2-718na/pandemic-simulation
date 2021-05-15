#pragma once

class Entity;

struct AI {
  // Utils /////////////////////////////////////////////////////////////////////
  static int randInt(int min, int max);
  static bool chanceCheck(float chance);

  // Static AI /////////////////////////////////////////////////////////////////
  static void nullAi(Entity* _this, int time = 0);
  static void randomAi(Entity* _this, int time = 0);
  static void testAi(Entity* _this, int time);
};