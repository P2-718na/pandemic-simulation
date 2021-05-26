#ifndef AI_GRAD_HPP
#define AI_GRAD_HPP

#include "entity.hpp"

class gradAI : public AI {
  inline void operator()(Entity* entity, int time, int day) override {
    if (entity->quarantined()) {
      entity->goHome();
      return;
    }

    const Config& config = entity->config();

    // On weekends, behave like an university student
    if (day >= entity->config().DAYS_IN_A_WEEK - 2) {
      // Go party at night
      if (time > config.hourToMinutes(21)) {
        entity->goParty();
        return;
      }
      // Go to random places all day
      if (time > config.hourToMinutes(9)) {
        switch (Config::randInt(0, 3)) {
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
            return;
        }
      }

      entity->goHome();
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

    // Added so it can get home after sunday night.
    entity->goHome();
  }
};

#endif // define GRAD_HPP
