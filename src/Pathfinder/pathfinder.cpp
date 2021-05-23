#include "pathfinder.hpp"
#include "config.hpp"

#include <algorithm>
#include <cmath>
// A* algorithm from https://www.geeksforgeeks.org/a-search-algorithm/

aStarList Pathfinder::aStarFullList_;

void Pathfinder::loadMap(const Config& config, sf::Image map) {
  const int width = map.getSize().x;
  const int height = map.getSize().y;

  std::vector<std::vector<aStarNode*>> tempNodeMap(width, std::vector<aStarNode*>(height, nullptr));

  // Phase 1:
  // load node weights...
  for (int column = 0; column != width; ++column) {
    for (int row = 0; row != height; ++row) {
      const sf::Color& pixelColor = map.getPixel(column, row);

      const int weight = config.A_STAR_WEIGHT_BY_COLOR(pixelColor);

      if (weight > 0) {
        aStarFullList_.emplace_back(Coords{column, row}, weight);
        tempNodeMap[column][row] = &aStarFullList_.back();
      }
    }
  }

  // Phase 2:
  // assign node neigbors...
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
        aStarNode* neighborPtr = tempNodeMap[neighborX][neighborY];
        if (neighborPtr == nullptr) {
          continue;
        }

        // Finally, we can add this neigbor to node
        node.neigbors.push_back(neighborPtr);
      }
    }
  }
}

auto Pathfinder::aStarFindLowestF_(const aStarList& list) {
  auto compareNodeF = [](const aStarNode& a, const aStarNode& b) {
    return a.f() < b.f();
  };

  return std::min_element(list.begin(), list.end(), compareNodeF);
}

auto Pathfinder::nodeWithLowerFInList_(const aStarNode& node, const aStarList& list) {
  auto compareNodes = [node](const aStarNode& listNode) {
    return listNode.coords == node.coords && listNode.f() < node.f();
  };

  return std::find_if(list.begin(), list.end(), compareNodes);
}

int Pathfinder::aStarComputeHeuristics_(const Coords& nodeCoords) const {
  // Diagonal distance heuristics
  return std::max(std::abs(nodeCoords.first - endCoords_.first),
    abs(nodeCoords.second - endCoords_.second));
}

void Pathfinder::reset() {
  // 1. Initialize open list
  aStarOpenList_.clear();

  // 2. Initialize closed list
  aStarClosedList_ = aStarFullList_;
}

void Pathfinder::init(const Coords& startCoords, const Coords& endCoords) {
  startCoords_ = startCoords;
  endCoords_ = endCoords;

  reset();
}

aStarNode* Pathfinder::generateTree() {
  auto compareNodeCoords = [this](aStarNode& node) {
    return node.coords == startCoords_;
  };

  // 2.a Put starting node on the open list
  auto startingNode = find_if(aStarClosedList_.begin(), aStarClosedList_.end(),
    compareNodeCoords);  // todo error handling
  aStarOpenList_.splice(aStarOpenList_.begin(), aStarClosedList_, startingNode);

  // 3. While the open list is not empty...
  while (!aStarOpenList_.empty()) {
    // 3.a find node with least f on open list and call it q.
    // q is an iterator to node with lowest F on Open List.
    auto q = aStarFindLowestF_(aStarOpenList_);

    // 3.b && 3.e pop q out off the open list and push it to the closed list.
    aStarClosedList_.splice(aStarClosedList_.end(), aStarOpenList_, q);

    // 3.d For each neighbor...
    //todo rewrite to use vector....
    for (int i = 0; i != 8; ++i) {
      auto neigborPtr = q->neigbors[i];

      // 3.c set its parent to q (last element of aStarClosedList)
      neigborPtr->parent = &aStarClosedList_.back();

      // 3.d.I If neigbor is goal, end search
      if (neigborPtr->coords == endCoords_) {
        return neigborPtr;
      }

      // Compute g and h values for neigbor
      neigborPtr->g = q->g + q->weight;
      neigborPtr->h = aStarComputeHeuristics_(neigborPtr->coords);

      // 3.d.II If a node with same position as neighbor but with lower f is
      // in open list, skip this neigbor
      if (nodeWithLowerFInList_(*neigborPtr, aStarOpenList_) != aStarOpenList_.end()) {
        continue;
      }

      // 3.d.III If a node with same position as neighbor but with lower f is
      // in closed list, add node to open list
      if (nodeWithLowerFInList_(*neigborPtr, aStarClosedList_) != aStarClosedList_.end()) {
        continue;
      }

      // Otherwise, add node to open list
      aStarOpenList_.push_back(*neigborPtr);
    }
  }

  return &(*startingNode);
}

void Pathfinder::computeAStar() {
  aStarNode* currentNode = generateTree();
  do {
    path_.push_back(currentNode->coords);
    currentNode = currentNode->parent;
  } while (currentNode->parent != nullptr);
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
