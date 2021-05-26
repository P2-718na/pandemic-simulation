#ifndef AI_UNI_HPP
#define AI_UNI_HPP

#include "entity.hpp"

class uniAI : public AI {
  inline void operator()(Entity* entity, int time, int day) override {
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
};

#endif // define UNI_HPP
