#include "ai.hpp"
#include "entity.hpp"
#include "config.hpp"

namespace AI {

void nullAI(Entity* entity, int minute, int day) {}

void randomAI(Entity* entity, int minute, int day) {
  if (entity->quarantined()) {
    return entity->goHome();
  }

  const int x = Config::randInt(100, 700);
  const int y = Config::randInt(100, 700);
  entity->setDestination(x, y);
}

}
