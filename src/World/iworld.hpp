#pragma once
#include <utility>

#include "entity.hpp"

class Entity;

class IWorld {
 public:
  // Accessors /////////////////////////////////////////////////////////////////
  virtual int time() const = 0;
  virtual std::pair<int, int> randomWalkCoords() = 0;
  virtual std::pair<int, int> randomShopCoords() = 0;
  virtual std::pair<int, int> randomPartyCoords() = 0;
};