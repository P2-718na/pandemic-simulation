#ifndef AI_HPP
#define AI_HPP
#include <memory>

class Entity;

// Fixme ask this: is it good to have stuff on base class?
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
  //todo add noexcept everywhere
  explicit AI(Entity* parent);
  virtual ~AI() noexcept = 0;
  virtual void operator()(int time, int day) = 0;
};

typedef std::unique_ptr<AI> entityAI;

#endif // define AI_HPP
