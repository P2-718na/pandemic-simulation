#ifndef PATHFINDER_HPP
#define  PATHFINDER_HPP

#include "micropather.h"
#include "map.hpp"

typedef std::pair<int, int> Coords;

class Pathfinder {
  // can't use reference for some reason
  static Map* map_;

  // Current path
  MP_VECTOR<State> path_;

  // Current step count.
  int step_{-1};

 public:
  static void setMap(Map* map) noexcept;

  // Set start and end positions. Calls reset() method.
  void loadPath(const Coords& startCoords, const Coords& endCoords);

  Coords step() noexcept;

  bool arrived() const noexcept;
};

#endif // define PATHFINDER_HPP
