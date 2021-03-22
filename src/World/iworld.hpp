#pragma once
#include "entity.hpp"

class Entity;

class IWorld {
 public:
  // Accessors /////////////////////////////////////////////////////////////////
  virtual int time() const = 0;
};