#ifndef AI_GRAD_HPP
#define AI_GRAD_HPP

#include "entity.hpp"

namespace pandemic {

class gradAI : public AI {
 public:
  explicit inline gradAI(Entity* parent) : AI{parent} {}

  inline void operator()(int time, int day) override {
    if (stayHome_(day)) {
      parent_->goHome();
      return;
    }

    const Config& config = parent_->config();

    // On weekends, behave like an university student
    if (day >= parent_->config().DAYS_IN_A_WEEK - 2) {
      // Go party at night
      if (time > config.hourToMinutes(21)) {
        parent_->goParty();
        return;
      }
      // Go to random places all day
      if (time > config.hourToMinutes(9)) {
        switch (Config::randInt(0, 3)) {
          case 0:
            parent_->goWalk();
            return;
          case 1:
            parent_->goShop();
            return;
          case 2:
            parent_->goParty();
            return;
          default:
            return;
        }
      }

      parent_->goHome();
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

    // Added so it can get home after sunday night.
    parent_->goHome();
  }
};

} // namespace pandemic

#endif // define GRAD_HPP
