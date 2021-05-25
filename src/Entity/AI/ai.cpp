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
void manAI(Entity* entity, int time, int day) {
  if (entity->quarantined()) {
    return entity->goHome();
  }

  const Config& config = entity->config();

  // During weekdays...
  if (day < config.DAYS_IN_A_WEEK - 2) {
    // Gets home at 18
    if (time > config.hourToMinutes(18)) {
      entity->goHome();
      return;
    }
    // Goes to work at 9
    if (time > config.hourToMinutes(9)) {
      entity->goWork();
      return;
    }
  }

  // on Saturday
  if (day == config.DAYS_IN_A_WEEK - 2) {
    // Relax
    if (time > config.hourToMinutes(15)) {
      entity->goWalk();
      return;
    }
  }

  // on Sunday
  if (day == config.DAYS_IN_A_WEEK - 1) {
    // Go home
    if (time > config.hourToMinutes(13)) {
      entity->goHome();
      return;
    }
    // Go shopping
    if (time > config.hourToMinutes(10)) {
      entity->goShop();
      return;
    }
  }
}


void oldAI(Entity* entity, int time, int day) {
  // Se è in quarantena, sta a casa.
  if (entity->quarantined()) {
    return entity->goHome();
  }

  const Config& config = entity->config();

  // Alle 18 torna a casa-
  if (time > config.hourToMinutes(18)) {
    entity->goHome();
    return;
  }
  // Alle 17 va a fare la spesa.
  if (time > config.hourToMinutes(17)) {
    entity->goShop();
    return;
  }
  // Alle 10 torna a casa e pranza.
  if (time > config.hourToMinutes(10)) {
    entity->goHome();
    return;
  }
  // Si sveglia alle 5 e va a passeggiare.
  if (time > config.hourToMinutes(5)) {
    entity->goWalk();
    return;
  }
}

}
