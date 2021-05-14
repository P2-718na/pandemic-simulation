#pragma once
#include <utility>
class IWorld {
 public:
  // Accessors /////////////////////////////////////////////////////////////////
  virtual int time() const = 0;
  virtual std::pair<int, int> randomParkCoords() = 0;
  virtual std::pair<int, int> randomShopCoords() = 0;
  virtual std::pair<int, int> randomPartyCoords() = 0;
};
