#ifndef PANDEMIC_PATHFINDER_HPP
#define PANDEMIC_PATHFINDER_HPP

#include <vector>

#include "types.hpp"

namespace pandemic {

class Pathfinder {
  // Path to take to get to destination
  std::vector<Coords> path_{};

  // Current step in path_
  int step_{ -1 };

 public:
  // Constructor ///////////////////////////////////////////////////////////////
  explicit Pathfinder(int size = 0);

  // Methods ///////////////////////////////////////////////////////////////////
  void loadPath(const Coords& startPos, const Coords& endPos) noexcept;

  // Accessors /////////////////////////////////////////////////////////////////
  // Unused, left here for testing purposes.
  const std::vector<Coords>& getPath() const;

  // Get next step in path. If arrived, return last step.
  const Coords& step();

  // True if we arrived at the last step.
  bool arrived();
};

}  // namespace pandemic

#endif  // define PANDEMIC_PATHFINDER_HPP
