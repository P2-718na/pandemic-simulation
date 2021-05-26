#ifndef AI_MAN_HPP
#define AI_MAN_HPP

#include "entity.hpp"

class manAI : public AI {
  inline void operator()(Entity* entity, int time, int day) override {
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
  }
};

#endif // define MAN_HPP
