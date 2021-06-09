#ifndef PANDEMIC_RANDOM_HPP
#define PANDEMIC_RANDOM_HPP

#include "entity.hpp"
#include "config.hpp"

namespace pandemic {

class randomAI : public AI {
 public:
  explicit inline randomAI(Entity* parent)
    : AI{ parent } {}

  inline void operator()(int time, int day) noexcept override {
    if (stayHome_(day)) {
      parent_->goHome();
      return;
    }

    const int x = Config::randInt(100, 700);
    const int y = Config::randInt(100, 700);
    parent_->setDestination({ x, y });
  }
};

}  // namespace pandemic

#endif  // define PANDEMIC_UNI_HPP
