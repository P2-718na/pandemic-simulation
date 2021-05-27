#ifndef AI_RANDOM_HPP
#define AI_RANDOM_HPP

#include "entity.hpp"

class randomAI : public AI {
  inline void operator()(Entity* entity, int time, int day) override {
    if (entity->quarantined()) {
      return entity->goHome();
    }

    const int x = Config::randInt(100, 700);
    const int y = Config::randInt(100, 700);
    entity->setDestination({x, y});
  }
};

#endif // define UNI_HPP
