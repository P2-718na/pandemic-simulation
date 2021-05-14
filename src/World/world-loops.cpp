#include "world.hpp"

void World::_dayLoop() {
  ++currentDay_;
  currentMinute_ = 0;

  for (auto &entity : this->entities) {
    entity.dayLoop();
  }
}

void World::loop() {
  ++currentMinute_;

  // Empty _activeTiles, since they will be regenerated.
  this->activeTiles_.clear();

  // Loop every entity and update position on _map array
  for (auto &entity : this->entities) {
    this->_entityPreLoop(entity);
    entity.loop();
    this->_entityPostLoop(entity);
  }

  // For each entity in tile, spread infection. Note that active tiles are the
  // One where at least one infected person exists, for now.
  // Also note that an entity must succeed a spread check for a tile to
  // be considered active
  for (auto &tilePtr : this->activeTiles_) {
    for (auto &entityPtr : tilePtr->entities) {
      entityPtr->tryInfect();
    }
  }

  // Execute next day logic (Must be called last)
  if (this->currentMinute_ >= MINUTES_IN_A_DAY) {
    return this->_dayLoop();
  }
}

// Loops ///////////////////////////////////////////////////////////////////////
void World::_entityPreLoop(Entity &entity) {
  // If entity is in a valid position, clear it from the board
  if (isInside(entity)) {
    // get tile the entity is in pre-loop
    Tile *currentTile = &this->map_[entity.posX()][entity.posY()];

    // remove from map.
    currentTile->entities.clear();
    currentTile->entityCount = 0;
  }
}

void World::_entityPostLoop(Entity &entity) {
  // If entity is in a valid position, update board
  if (isInside(entity)) {
    // get tile the entity should be in post-loop
    Tile *currentTile = &this->map_[entity.posX()][entity.posY()];

    // increase its entity count  and add to active tiles
    // Count tile with only one entity as inactive, for now.
    // Also note that a tile must succeed a spread check in order to make a tile
    // active.
    ++(currentTile->entityCount);
    if (
      currentTile->entityCount > 1 && entity.infective() &&
      AI::chanceCheck(entity.virusSpreadChance)
      ) {
      this->activeTiles_.push_back(&map_[entity.posX()][entity.posY()]);
    }

    // add in map
    currentTile->entities.push_back(&entity);
  }
}
