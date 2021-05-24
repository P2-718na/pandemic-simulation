#include <SFML/Graphics/Image.hpp>
#include <iostream>

#include "doctest/doctest.h"
#include "pathfinder.hpp"
#include "config.hpp"

std::ostream& operator<<(std::ostream& os, const Coords& coords) {
  os << '(' << coords.first << ", " << coords.second << ')';
  return os;
}


TEST_CASE("Pathfinder") {
  Config config{};
  Pathfinder pathfinder;
  sf::Image img;

  REQUIRE(img.loadFromFile("./background.test.bmp"));

  pathfinder.loadMap(config, img);

  pathfinder.computePath({0, 0}, {4, 5});
  pathfinder.computeAStar();

  do {
    std::cout << pathfinder.step() << std::endl;
  } while (!pathfinder.arrived());
}
