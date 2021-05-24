#include <iostream>
#include <cmath>
#include <vector>

#include "map.hpp"
#include "config.hpp"

int Map::height_;
int Map::width_;

Coords Map::stateToCoords(State state) noexcept {
  long stateL = reinterpret_cast<long>(state);
  return {stateL % width_, stateL / width_};
}

State Map::coordsToState(const Coords& coords) noexcept {
  return reinterpret_cast<void*>(coords.second * width_ + coords.first);
}

// MicroPather /////////////////////////////////////////////////////////////////
float Map::LeastCostEstimate(void* stateStart, void* stateEnd) {
  const Coords startCoords = stateToCoords(stateStart);
  const Coords endCoords = stateToCoords(stateEnd);

  return sqrt(pow(startCoords.first - endCoords.first, 2)
              + pow(startCoords.second - endCoords.second, 2));
}

void Map::AdjacentCost(void* state, MP_VECTOR<mp::StateCost>* adjacent) {
  const Coords stateCoords = stateToCoords(state);

  *adjacent = grid_[stateCoords.first][stateCoords.second];
}

void Map::PrintStateInfo(void* state) {
  const Coords stateCoords = stateToCoords(state);

  std::cout << "(" << stateCoords.first << ", " << stateCoords.second << ")";
}

// Public //////////////////////////////////////////////////////////////////////
void Map::init(const Config& config, const sf::Image& map) {
  width_ = map.getSize().x;
  height_ = map.getSize().y;

  grid_ = CostGrid(width_, Column(height_));

  std::vector<std::vector<float>> tempGrid(width_, std::vector<float>(height_, -1));

  // Phase 1:
  // load node weights...
  for (int x = 0; x != width_; ++x) {
    for (int y = 0; y != height_; ++y) {
      const sf::Color& pixelColor = map.getPixel(x, y);

      const int weight = config.A_STAR_WEIGHT_BY_COLOR(pixelColor);

      if (weight > 0) {
        tempGrid[x][y] = weight;
      }
    }
  }

  // Phase 2:
  // assign node neigbors...
  for (int x = 0; x != width_; ++x) {
    for (int y = 0; y != height_; ++y) {
      // For each node, loop through every possible neighbor
      for (int i = -1; i != 2; ++i) {
        for (int j = -1; j != 2; ++j) {
          // We don't want a node to be a neighbor of itself.
          if (i == 0 && j == 0) {
            continue;
          }

          // Also we don't want to compute neighbors for nodes not in the map
          if (tempGrid[x][y] <= 0) {
            continue;
          }

          // Possible neighbor coordinates
          const int neighborX = x + i;
          const int neighborY = y + j;

          // Coords must be positive and have to be inside tempGrid...
          if (neighborY < 0 || neighborY >= height_ || neighborX < 0
              || neighborX >= width_) {
            continue;
          }

          // And we need to check that the weight at that coordinates is not
          // -1
          const int neighborWeight = tempGrid[neighborX][neighborY];
          if (neighborWeight == -1) {
            continue;
          }

          // Finally, we can add this neighbor to grid
          const State neighborState = coordsToState({neighborX, neighborY});
          grid_[x][y].push_back({neighborState, tempGrid[neighborX][neighborY]});
        }
      }
    }
  }
}

// Accessors ///////////////////////////////////////////////////////////////////
int Map::width() noexcept {
  return width_;
}
int Map::height() noexcept {
  return height_;
}
mp::MicroPather& Map::pather() noexcept {
  return pather_;
}
