#ifndef PANDEMIC_TEEN_HPP
#define PANDEMIC_TEEN_HPP

#include "entity.hpp"

namespace pandemic {

class teenAI : public AI {
 public:
  inline explicit teenAI(Entity* parent)
    : AI{ parent } {}

  inline void operator()(int time, int day) noexcept override {
    if (stayHome_(day)) {
      parent_->goHome();
      return;
    }

    const Config& config = parent_->config();

    // on Saturday
    if (day == config.DAYS_IN_A_WEEK() - 2) {
      // Go party for the rest of the day
      if (time > config.hourToMinutes(20)) {
        parent_->goParty();
        return;
      }
      // Either go party or go shop in afternoon
      if (time > config.hourToMinutes(15)) {
        Config::chanceCheck(0.5) ? parent_->goParty() : parent_->goShop();
        return;
      }
      // Get home after school
      if (time > config.hourToMinutes(13)) {
        parent_->goHome();
        return;
      }
      // Go to school
      if (time > config.hourToMinutes(7.5)) {
        parent_->goWork();
        return;
      }

      return;
    }

    // on Sunday
    if (day == config.DAYS_IN_A_WEEK() - 1) {
      // Get back home
      if (time > config.hourToMinutes(19.5)) {
        parent_->goHome();
        return;
      }
      // Either go party or go shop
      if (time > config.hourToMinutes(15)) {
        Config::chanceCheck(0.5) ? parent_->goParty() : parent_->goShop();
        return;
      }
      // Go home
      if (time > config.hourToMinutes(3)) {
        parent_->goHome();
        return;
      }

      // Keep partying
      parent_->goParty();
      return;
    }

    // On weekdays...
    // Gets home at 18
    if (time > config.hourToMinutes(13)) {
      parent_->goHome();
      return;
    }
    // Goes to school
    if (time > config.hourToMinutes(7.5)) {
      parent_->goWork();
      return;
    }
  }
};

}  // namespace pandemic

#endif  // define PANDEMIC_TEEN_HPP
