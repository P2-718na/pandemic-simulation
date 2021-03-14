#include "pathfinder.hpp"
#include <stdexcept>

void Pathfinder::_init(const int &startX, const int &startY, const int &endX, const int &endY) {
  if (this->_step != -1) {
   throw std::runtime_error("Already initialised.");
  }

  this->_startX = startX;
  this->_startY = startY;
  this->_endX = endX;
  this->_endY = endY;
  this->_step = 0;

  _calcPath();
}

bool Pathfinder::_calcPath() {
  // don't add first node
  // this->_path.push_back(new int[2] {this->_startX, this->_startY});

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
    } else if (currentY > this->_endY) {
      --currentY;
    }

    this->_path.push_back(new int[2] {currentX, currentY});
  }

  // add last node
  this->_path.push_back(new int[2] {this->_endX, this->_endY});

  return true;
}

Pathfinder::Pathfinder() = default;

Pathfinder::Pathfinder(
  const int &startX,
  const int &startY,
  const int &endX,
  const int &endY
) {
  this->_init(startX, startY, endX, endY);
}

Pathfinder::~Pathfinder() {
  for (auto i : this->_path) {
    delete[] i;
  }
}

void Pathfinder::init(
  const int &startX,
  const int &startY,
  const int &endX,
  const int &endY
) {
  return this->_init(startX, startY, endX, endY);
}

std::vector<int *> Pathfinder::getPath() const {
  return this->_path;
}

int* Pathfinder::step() {
  if (this->_step == -1) {
    throw std::runtime_error("Pathfinder not initialised.");
  }

  const int totalStepCount = (int)this->_path.size();
  if (this->_step >= totalStepCount) {
    return this->_path[totalStepCount];
  }

  return this->_path[this->_step++];
}

bool Pathfinder::isArrived() {
  return this->_step >= this->_path.size() - 1;
}