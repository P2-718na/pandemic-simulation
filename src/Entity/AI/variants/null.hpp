#ifndef PANDEMIC_NULL_HPP
#define PANDEMIC_NULL_HPP

#include "entity.hpp"

namespace  pandemic {

class nullAI : public AI {
 public:
  explicit inline nullAI(Entity* parent) : AI{parent} {}

  inline void operator()(int time, int day) override {}
};

} // namespace pandemic

#endif // define PANDEMIC_NULL_HPP
