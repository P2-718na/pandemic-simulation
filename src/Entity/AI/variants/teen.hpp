#ifndef AI_TEEN_HPP
#define AI_TEEN_HPP

#include "entity.hpp"

class teenAI : public AI {
  inline void operator()(Entity* entity, int time, int day) override {
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
};

#endif // define TEEN_HPP
