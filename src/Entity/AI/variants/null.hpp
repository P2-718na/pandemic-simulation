#ifndef AI_NULL_HPP
#define AI_NULL_HPP

#include "entity.hpp"

class nullAI : public AI {
  inline void operator()(Entity* entity, int time, int day) override {}
};

#endif // define UNI_HPP
