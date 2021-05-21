#include "pathfinder.hpp"

#include <algorithm>
#include <cmath>
// A* algorithm from https://www.geeksforgeeks.org/a-search-algorithm/

void Pathfinder::loadMap(sf::Image map) {

  const int width = map.getSize().x;
  const int height = map.getSize().y;

  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {

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
  // 2.a Put starting node on the open list
  auto startingNode = find(aStarClosedList_.begin(), aStarClosedList_.end(),
    startCoords_);  // todo error handling
  aStarOpenList_.splice(aStarOpenList_.begin(), aStarClosedList_, startingNode);

  // 3. While the open list is not empty...
  while (!aStarOpenList_.empty()) {
    // 3.a find node with least f on open list and call it q.
    // q is an iterator to node with lowest F on Open List.
    auto q = aStarFindLowestF_(aStarOpenList_);

    // 3.b && 3.e pop q out off the open list and push it to the closed list.
    aStarClosedList_.splice(aStarClosedList_.end(), aStarOpenList_, q);

    // 3.d For each neighbor...
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
}

void Pathfinder::computeAStar() {
  aStarNode* currentNode = generateTree();
  do {
    _path.push_back(currentNode->coords);
    currentNode = currentNode->parent;
  } while (currentNode->parent != nullptr);
}
