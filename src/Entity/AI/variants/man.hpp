#ifndef PANDEMIC_MAN_HPP
#define PANDEMIC_MAN_HPP

#include "entity.hpp"

namespace pandemic {

class manAI : public AI {
 public:
  explicit inline manAI(Entity* parent)
    : AI{ parent } {}

  inline void operator()(int time, int day) noexcept override {
    if (stayHome_(day)) {
      parent_->goHome();
      return;
    }

    const Config& config = parent_->config();

    // on Saturday
    if (day == config.DAYS_IN_A_WEEK() - 2) {
      // Get back home at night
      if (time > config.hourToMinutes(18)) {
        parent_->goHome();
        return;
      }
      // Relax
      if (time > config.hourToMinutes(15)) {
        parent_->goWalk();
        return;
      }

      return;
    }

    // on Sunday
    if (day == config.DAYS_IN_A_WEEK() - 1) {
      // Go home
      if (time > config.hourToMinutes(13)) {
        parent_->goHome();
        return;
      }
      // Go shopping
      if (time > config.hourToMinutes(10)) {
        parent_->goShop();
        return;
      }

      return;
    }

    // During weekdays...
    // Gets home at 18
    if (time > config.hourToMinutes(18)) {
      parent_->goHome();
      return;
    }
    // Goes to work at 9
    if (time > config.hourToMinutes(9)) {
      parent_->goWork();
      return;
    }
  }
};

}  // namespace pandemic

#endif  // define MAN_HPP
