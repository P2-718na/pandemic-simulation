#include <algorithm>

#include "world.hpp"

void World::dayLoop_() {
  ++currentDay_;
  currentMinute_ = 0;

  for (auto &entity : this->entities_) {
    entity.dayLoop();

    handleQuarantine_(entity);
  }

  printf("New day! %d\nInfected: %d, Dead: %d\n", currentDay_, infectedCount(), deadCount());
}

void World::loop() {
  ++currentMinute_;

  // Loop every entity
  for (auto &entity : this->entities_) {
    entity.loop();
  }

  // Spread virus
  spreadVirus_();

  // Execute next day logic (Must be called last)
  if (this->currentMinute_ >= this->config_.MINUTES_IN_A_DAY) {
    dayLoop_();
  }
}
