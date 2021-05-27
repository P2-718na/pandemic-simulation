#ifndef AI_NULL_HPP
#define AI_NULL_HPP

#include "entity.hpp"

class nullAI : public AI {
 public:
  explicit inline nullAI(Entity* parent) : AI{parent} {}

  inline void operator()(int time, int day) override {}
};

#endif // define UNI_HPP
