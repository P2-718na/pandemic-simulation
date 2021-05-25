#pragma once

class Entity;

// Entity*, points to current entity, int is the current time of day.
typedef void (*entityAI)(Entity*, int);

struct AI {
  // Utils /////////////////////////////////////////////////////////////////////
  static int randInt(int min, int max);
  static bool chanceCheck(float chance);

  // Static AI /////////////////////////////////////////////////////////////////
  static void nullAI(Entity* _this, int time = 0);
  static void randomAI(Entity* _this, int time = 0);
  static void testAI(Entity* _this, int time);
};
