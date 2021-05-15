#include <algorithm>

#include "world.hpp"

void World::dayLoop_() {
  ++currentDay_;
  currentMinute_ = 0;

  for (auto &entity : this->entities) { entity.dayLoop(); }

  printf("New day! %d\nInfected: %d\n", currentDay_, infectedCount());
}

void World::loop() {
  ++currentMinute_;

  // Loop every entity
  for (auto &entity : this->entities) { entity.loop(); }

  // Spread virus
  spreadVirus_();

  // Execute next day logic (Must be called last)
  if (this->currentMinute_ >= MINUTES_IN_A_DAY) {
    dayLoop_();
  }
}

void World::spreadVirus_() {
  std::vector<Coords> infectiveTiles;

  // Save coordinates for each infective entity...
  for (auto &infectiveEntity : this->entities) {
    if (!infectiveEntity.infective()) {
      continue;
    }

    // An infective entity has to succeed a virus spread test
    if (AI::chanceCheck(infectiveEntity.virusSpreadChance)) {
      infectiveTiles.emplace_back(
        infectiveEntity.posX(), infectiveEntity.posY());
    }
  }

  // And spread virus
  for (auto &entity : this->entities) {
    // Map is not very efficient compared to this
    const auto &first = infectiveTiles.begin();
    const auto &last = infectiveTiles.end();
    const bool contactWithInfected = std::find(first, last, entity.pos()) != last;
    if (contactWithInfected) {
      entity.tryInfect();
    }
  }
}
