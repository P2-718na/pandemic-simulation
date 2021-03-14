#include "pathfinder.hpp"

bool Pathfinder::_calcPath() {
  // add first node
  this->_path.push_back(new int[2] {this->_startX, this->_startY});

  int currentX = this->_startX;
  int currentY = this->_startY;

  while (currentX != this->_endX || currentY != this->_endY) {
    if (currentX < this->_endX) {
      ++currentX;
    } else if (currentX > this->_endX) {
      --currentX;
    }

    if (currentY < this->_endY) {
      ++currentY;
    } else if (currentX > this->_endY) {
      --currentY;
    }

    this->_path.push_back(new int[2] {currentX, currentY});
  }

  // add last node
  this->_path.push_back(new int[2] {this->_endX, this->_endY});

  return true;
}

Pathfinder::Pathfinder(int startX, int startY, int endX, int endY)
  : _startX{startX}
  , _startY{startY}
  , _endX{endX}
  , _endY{endY}
{
  _calcPath();
}

Pathfinder::~Pathfinder() {
  for (auto i : this->_path) {
    delete[] i;
  }
}

std::vector<int *> Pathfinder::getPath() const {
  return this->_path;
}

int* Pathfinder::step() {
  ++(this->_step);

  const int lastStep = (int)this->_path.size() - 1;
  if (this->_step > lastStep) {
    return this->_path[lastStep];
  }

  return this->_path[this->_step];
}