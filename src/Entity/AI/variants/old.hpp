#ifndef PANDEMIC_OLD_HPP
#define PANDEMIC_OLD_HPP

#include "entity.hpp"

namespace pandemic {

class oldAI : public AI {
 public:
  explicit inline oldAI(Entity* parent)
    : AI{ parent } {}

  inline void operator()(int time, int day) noexcept override {
    if (stayHome_(day)) {
      parent_->goHome();
      return;
    }

    const Config& config = parent_->config();

    // Alle 18 torna a casa-
    if (time > config.hourToMinutes(18)) {
      parent_->goHome();
      return;
    }
    // Alle 17 va a fare la spesa.
    if (time > config.hourToMinutes(17)) {
      parent_->goShop();
      return;
    }
    // Alle 10 torna a casa e pranza.
    if (time > config.hourToMinutes(10)) {
      parent_->goHome();
      return;
    }
    // Si sveglia alle 5 e va a passeggiare.
    if (time > config.hourToMinutes(5)) {
      parent_->goWalk();
      return;
    }
  }
};

}  // namespace pandemic

#endif  // define PANDEMIC_OLD_HPP
