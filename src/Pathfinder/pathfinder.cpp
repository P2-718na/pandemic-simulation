#include <vector>
#include <cassert>

#include "pathfinder.hpp"

namespace pandemic {

// Constructor /////////////////////////////////////////////////////////////////
Pathfinder::Pathfinder(int size) {
  // Reserve an arbitrary space for paths. This will make
  // Pathfinder slightly faster in the beginning.
  //path_.reserve(size);
};

void Pathfinder::loadPath(const Coords& startPos, const Coords& endPos) noexcept {
  // Reset step and path
  step_ = 0;
  path_.clear();

  const int endX = endPos.first;
  const int endY = endPos.second;

  int currentX = startPos.first;
  int currentY = startPos.second;

  // don't add first node
  // Wile not arrived...
  while (!(currentX == endX && currentY == endY)) {
    if (currentX < endX) {
      ++currentX;
    } else if (currentX > endX) {
      --currentX;
    }

    if (currentY < endY) {
      ++currentY;
    } else if (currentY > endY) {
      --currentY;
    }

    path_.emplace_back(currentX, currentY);
  }
}

const std::vector<Coords>& Pathfinder::getPath() const {
  return path_;
}

const Coords& Pathfinder::step() {
  assert(step_ != -1);

  if (step_ >= path_.size()) {
    return path_[path_.size() - 1];
  }

  // Return path_[step] and increment it.
  return path_[step_++];
}

bool Pathfinder::arrived() {
  // Last element in path is the destination. If we get there, we are arrived.
  // Also, step is incremented after returning its corresponding coords in path,
  // which means that we arrive once step == path.size.
  return step_ >= path_.size();
}

} // namespace pandemic
