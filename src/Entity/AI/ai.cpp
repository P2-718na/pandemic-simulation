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
    entity->goHome();
    return;
  }

  const Config& config = entity->config();

  // on Saturday
  if (day == config.DAYS_IN_A_WEEK - 2) {
    // Relax
    if (time > config.hourToMinutes(15)) {
      entity->goWalk();
      return;
    }

    return;
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

    return;
  }

  // During weekdays...
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

  // Added for gradAi, so it can get home after sunday night.
  entity->goHome();
}


void oldAI(Entity* entity, int time, int day) {
  // Se è in quarantena, sta a casa.
  if (entity->quarantined()) {
    entity->goHome();
    return;
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

void gradAI(Entity* entity, int time, int day) {
  // On weekends, behave like an university student
  if (day >= entity->config().DAYS_IN_A_WEEK - 2) {
    uniAI(entity, time, day);
    return;
  }

  // On weekdays, behave like a man.
  manAI(entity, time, day);
}

void uniAI(Entity* entity, int time, int day) {
  if (entity->quarantined()) {
    entity->goHome();
    return;
  }

  const Config& config = entity->config();
  const bool weekend = day >= config.DAYS_IN_A_WEEK - 3;

  // Gets home at 21 on weekdays. Otherwise go party.
  if (time > config.hourToMinutes(21)) {
     weekend ?  entity->goParty() : entity->goHome();
    return;
  }
  // Goes to uni/party/shop/walk for the entire day.
  if (time > config.hourToMinutes(9)) {
    // Don't go to uni on weekends. Also, uni has a much higher chance of
    // happening than other actions, since we want entities to stay there a bit.
    switch (Config::randInt(0, weekend ? 3 : 200)) {
      case 0:
        entity->goWalk();
        return;
      case 1:
        entity->goShop();
        return;
      case 2:
        entity->goParty();
        return;
      default:
        entity->goWork();
        return;
    }
  }

  entity->goHome();
}

void teenAI(Entity* entity, int time, int day) {
  if (entity->quarantined()) {
    entity->goHome();
    return;
  }

  const Config& config = entity->config();

  // on Saturday
  if (day == config.DAYS_IN_A_WEEK - 2) {
    // Go party for the rest of the day
    if (time > config.hourToMinutes(20)) {
      entity->goParty();
      return;
    }
    // Either go party or go shop in afternoon
    if (time > config.hourToMinutes(15)) {
      Config::chanceCheck(0.5) ? entity->goParty() : entity->goShop();
      return;
    }
    // Get home after school
    if (time > config.hourToMinutes(13)) {
      entity->goHome();
      return;
    }
    // Go to school
    if (time > config.hourToMinutes(7.5)) {
      entity->goWork();
      return;
    }

    return;
  }

  // on Sunday
  if (day == config.DAYS_IN_A_WEEK - 1) {
    // Get back home
    if (time > config.hourToMinutes(19.5)) {
      entity->goHome();
      return;
    }
    // Either go party or go shop
    if (time > config.hourToMinutes(15)) {
      Config::chanceCheck(0.5) ? entity->goParty() : entity->goShop();
      return;
    }
    // Go home
    if (time > config.hourToMinutes(3)) {
      entity->goHome();
      return;
    }

    // Keep partying
    entity->goParty();
    return;
  }

  // On weekdays...
  // Gets home at 18
  if (time > config.hourToMinutes(13)) {
    entity->goHome();
    return;
  }
  // Goes to school
  if (time > config.hourToMinutes(7.5)) {
    entity->goWork();
    return;
  }
}

}
