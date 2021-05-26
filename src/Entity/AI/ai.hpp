#ifndef AI_HPP
#define AI_HPP
#include <memory>

class Entity;

class AI {
 public:
  virtual void operator()(Entity* entity, int time, int day) = 0;
  virtual ~AI() = 0;
};

inline AI::~AI() = default;

typedef std::unique_ptr<AI> entityAI;

#endif // define AI_HPP
