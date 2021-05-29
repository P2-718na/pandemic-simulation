#include <iostream>
#include "../doctest.h"
#include "pathfinder.hpp"
#include "types.hpp"

using pandemic::Pathfinder;

TEST_CASE("Test pathfinder") {
  Pathfinder pathfinder;

  pathfinder.loadPath({0, 0}, {3, 5});

  CHECK_EQ(pathfinder.getPath().size(), 5);

  auto step = pathfinder.step();
  CHECK_EQ(step, pandemic::Coords{1, 1});
  step = pathfinder.step();
  CHECK_EQ(step, pandemic::Coords{2, 2});
  step = pathfinder.step();
  CHECK_EQ(step, pandemic::Coords{3, 3});
  step = pathfinder.step();
  CHECK_EQ(step, pandemic::Coords{3, 4});
  step = pathfinder.step();
  CHECK_EQ(step, pandemic::Coords{3, 5});
  step = pathfinder.step();
  CHECK_EQ(step, pandemic::Coords{3, 5});
}
