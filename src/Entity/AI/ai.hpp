#ifndef PANDEMIC_AI_HPP
#define PANDEMIC_AI_HPP
#include <memory>

namespace pandemic {

class Entity;

class AI {
 protected:
  Entity* parent_;

  // When on lockdown, people must still be able to go shop for groceries,
  // even tho the number of people around is smaller.
  // The easiest way to implement this is to load their AI only for one day of
  // the week, chosen at random. The rest of the time, they stay home.
  int lockdownFreeDay_;

  // Check whether current entity is in quarantine or if there is an
  // ongoing lockdown. If current day is lockdownFreeDay for entity,
  // treat it as if there were no lockdown).
  bool stayHome_(int day) const noexcept;

 public:
  explicit AI(Entity* parent) noexcept;
  virtual ~AI() noexcept = 0;
  virtual void operator()(int time, int day) noexcept = 0;
};

typedef std::unique_ptr<AI> entityAI;

} // namespace pandemic

#endif // define PANDEMIC_AI_HPP
