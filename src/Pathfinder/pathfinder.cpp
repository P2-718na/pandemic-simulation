#include "pathfinder.hpp"
#include "config.hpp"

#include <algorithm>
#include <cmath>
// A* algorithm from https://www.geeksforgeeks.org/a-search-algorithm/

void Pathfinder::loadMap(const Config& config, sf::Image map) {
  //todo
  const int width = map.getSize().x;
  const int height = map.getSize().y;

  std::vector<std::vector<int>> tempNodeMap(width, std::vector<int>(height, -1));

  // Phase 1:
  // load node weights...
  for (int column = 0; column != width; ++column) {
    for (int row = 0; row != height; ++row) {
      const sf::Color& pixelColor = map.getPixel(column, row);

      const int weight = config.A_STAR_WEIGHT_BY_COLOR(pixelColor);

      if (weight > 0) {
        aStarFullList_.emplace_back(Coords{column, row}, weight);
        tempNodeMap[column][row] = row * width + column;
      }
    }
  }

  // Phase 2:
  // assign node neigbors...
  // fixme this sets neighbor pointer to aStarFullList, but the algorithm copies them.
  //  to fix this, add neighbor offset and recalculate it.
  for (auto& node : aStarFullList_) {
    // Loop through every possible neighbor...
    for (int i = -1; i != 2; ++i) {
      for (int j = -1; j != 2; ++j) {
        // We don't want a node to be a neighbor of itself.
        if (i == 0 && j == 0) {
          continue;
        }

        // Possible neighbor coordinates
        const int neighborX = node.coords.first + i;
        const int neighborY = node.coords.second + j;

        // Coords must be positive and have to be inside tempNodeMap...
        if (neighborY < 0 || neighborY >= height || neighborX < 0 || neighborX >= width) {
          continue;
        }

        // And we need to check that the pointer at that coordinates is not nullptr
        const int neighborOffset = tempNodeMap[neighborX][neighborY];
        if (neighborOffset == -1) {
          continue;
        }

        // Finally, we can add this neigbor to node
        node.neighborOffset.push_back(neighborOffset);
      }
    }
  }
}


void Pathfinder::computePath(const Coords& startCoords, const Coords& endCoords) {
  //todo
  startCoords_ = startCoords;
  endCoords_ = endCoords;

  reset();
}

const Coords& Pathfinder::step() noexcept {
  if (step_ == -1) {
    // todo handle this
  }

  const int totalStepCount = path_.size();

  if (step_ >= totalStepCount) {
    return path_[totalStepCount];
  }

  return path_[step_++];
}

bool Pathfinder::arrived() const noexcept {
  return step_ >= path_.size() - 1;
}
