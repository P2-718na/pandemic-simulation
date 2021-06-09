#ifndef PANDEMIC_UNI_HPP
#define PANDEMIC_UNI_HPP

#include "entity.hpp"

namespace pandemic {

class uniAI : public AI {
 public:
  explicit inline uniAI(Entity* parent) : AI{parent} {}

  inline void operator()(int time, int day) noexcept override {
    if (stayHome_(day)) {
      parent_->goHome();
      return;
    }

    const Config& config = parent_->config();
    const bool weekend = day >= config.DAYS_IN_A_WEEK() - 3;

    // Gets home at 21 on weekdays. Otherwise go party.
    if (time > config.hourToMinutes(21)) {
      weekend ? parent_->goParty() : parent_->goHome();
      return;
    }
    // Goes to uni/party/shop/walk for the entire day.
    if (time > config.hourToMinutes(9)) {
      // Don't go to uni on weekends. Also, uni has a much higher chance of
      // happening than other actions, since we want entities to stay there
      // for a bit.
      switch (Config::randInt(0, weekend ? 3 : 200)) {
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
          parent_->goWork();
          return;
      }
    }

    parent_->goHome();
  }
};

}  // namespace pandemic

#endif  // define PANDEMIC_UNI_HPP
