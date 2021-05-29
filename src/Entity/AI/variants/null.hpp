#ifndef AI_NULL_HPP
#define AI_NULL_HPP

#include "entity.hpp"

namespace  pandemic {

class nullAI : public AI {
 public:
  explicit inline nullAI(Entity* parent) : AI{parent} {}

  inline void operator()(int time, int day) override {}
};

} // namespace pandemic

#endif // define UNI_HPP
