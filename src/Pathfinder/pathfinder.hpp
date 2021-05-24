#ifndef PATHFINDER_HPP
#define  PATHFINDER_HPP
#include <vector>
#include <list>
#include <SFML/Graphics/Image.hpp>

typedef std::pair<int, int> Coords;
class Config;

class Pathfinder {
  Coords startCoords_;
  Coords endCoords_;
  int step_{-1};

 public:
  //todo move back
  std::vector<Coords> path_{};

  static void loadMap(const Config& config, sf::Image map);

  // Set start and end positions. Calls reset() method.
  void computePath(const Coords& startCoords, const Coords& endCoords);

  const Coords& step() noexcept;

  bool arrived() const noexcept;
};

#endif // define PATHFINDER_HPP
