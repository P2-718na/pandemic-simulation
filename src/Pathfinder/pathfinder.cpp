#include "pathfinder.hpp"
#include "config.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

Map* Pathfinder::map_;

void Pathfinder::setMap(Map* map) noexcept {
  map_ = map;
}

void Pathfinder::loadPath(const Coords& startCoords, const Coords& endCoords) {
  const State startState = Map::coordsToState(startCoords);
  const State endState = Map::coordsToState(endCoords);

  path_.clear();
  step_ = 0;

  float _ = 0;
  // todo check for return value;
  const int result = map_->pather().Solve(startState, endState, &path_, &_);

  if (result != mp::MicroPather::SOLVED) {
    step_ = -1;
  }
}

Coords Pathfinder::step() noexcept {
  if (step_ == -1) {
    // todo handle this
  }

  const int totalStepCount = path_.size();

  if (step_ >= totalStepCount) {
    return Map::stateToCoords(path_[totalStepCount - 1]);
  }

  return Map::stateToCoords(path_[++step_]);
}

bool Pathfinder::arrived() const noexcept {
  return step_ >= path_.size() - 1;
}
