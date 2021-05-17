#pragma once
#include "config.hpp"

#include <utility>

typedef std::pair<int, int> Coords;

enum Day {
  sunday,
  monday,
  tuesday,
  wednesday,
  thursday,
  friday,
  saturday
};

class IWorld {
 public:
  IWorld(Config& config) : config{config} {};

  Config& config;

  // Accessors /////////////////////////////////////////////////////////////////
  virtual Day currentDay() const = 0;
  virtual int currentMinute() const = 0;
  virtual const Coords& randomParkCoords() = 0;
  virtual const Coords& randomShopCoords() = 0;
  virtual const Coords& randomPartyCoords() = 0;
};
