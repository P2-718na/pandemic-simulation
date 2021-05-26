#ifndef AI_OLD_HPP
#define AI_OLD_HPP

#include "entity.hpp"

class oldAI : public AI {
  inline void operator()(Entity* entity, int time, int day) override {
    // If quarantined, stay at home.
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
};

#endif // define OLD_HPP
