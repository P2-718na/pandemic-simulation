#include "ai.hpp"
#include "entity.hpp"
#include "config.hpp"

namespace AI {

// Default AI //////////////////////////////////////////////////////////////////
void nullAI(Entity* entity, int minute, int day) {}

void randomAI(Entity* entity, int minute, int day) {
  if (entity->quarantined()) {
    return entity->goHome();
  }

  const int x = Config::randInt(100, 700);
  const int y = Config::randInt(100, 700);
  entity->setDestination(x, y);
}

// Variants ////////////////////////////////////////////////////////////////////
void oldAI(Entity* entity, int time, int day) {
  if (entity->quarantined()) {
    return entity->goHome();
  }

  // Alle 18 torna a casa
  if (time > 6750) {
    return entity->goHome();
  }
  // Alle 17 va a fare la spesa
  if (time > 6375) {
    return entity->goShop();
  }
  // Alle 10 torna a casa e pranza
  if (time > 3600) {
    return entity->goHome();
  }
  // Si sveglia alle 5 e va a passeggiare
  if (time > 1800) {
    return entity->goWalk();
  }
}

}
