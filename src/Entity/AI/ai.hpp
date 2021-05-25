#pragma once

class Entity;

// Entity*, points to current entity, int is the current time of day.
typedef void (*entityAI)(Entity*, int);

struct AI {
  // Static AI /////////////////////////////////////////////////////////////////
  static void nullAI(Entity* _this, int time = 0);
  static void randomAI(Entity* _this, int time = 0);
  static void testAI(Entity* _this, int time);
};
