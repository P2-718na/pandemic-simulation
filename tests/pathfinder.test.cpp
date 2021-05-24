#include <SFML/Graphics/Image.hpp>
#include <iostream>

#include "doctest/doctest.h"
#include "pathfinder.hpp"
#include "config.hpp"
#include "map.hpp"

std::ostream& operator<<(std::ostream& os, const Coords& coords) {
  os << '(' << coords.first << ", " << coords.second << ')';
  return os;
}


TEST_CASE("Pathfinder") {
  Config config{};
  Pathfinder pathfinder;
  Map map;
  sf::Image img;

  REQUIRE(img.loadFromFile("./background.test.bmp"));

  map.init(config, img);
  Pathfinder::setMap(&map);

  pathfinder.loadPath({0, 0}, {3, 4});

  do {
    std::cout << pathfinder.step() << std::endl;
  } while (!pathfinder.arrived());
}
