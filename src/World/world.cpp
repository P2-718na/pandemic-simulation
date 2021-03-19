#include "world.hpp"
#include "ai.hpp"

#include <utility>

void World::_entityPreLoop(Entity &entity) {
  // If entity is in a valid position, update board
  if (isInside(entity)) {
    // get tile the entity is in pre-loop
    Tile *currentTile = &this->_map[entity.posX()][entity.posY()];

    // decrease its value
    // and remove from active tiles if empty.
    // Count tile with only one entity as inactive, for now
    if (--(currentTile->entityCount) <= 1) {
      // todo maybe some error handling?
      this->_activeTiles.erase({entity.posX(), entity.posY()});
    }

    // remove from map
    currentTile->entities.erase(&entity);
  }
}

void World::_entityPostLoop(Entity &entity) {
  // If entity is in a valid position, update board
  if (isInside(entity)) {
    // get tile the entity should be in post-loop
    Tile *currentTile = &this->_map[entity.posX()][entity.posY()];

    // increase its value
    // and add to active tiles
    // Count tile with only one entity as inactive, for now
    // todo replace daysinfected with actual .infected funcion (see Entity)
    if (++(currentTile->entityCount) > 1 && entity.daysInfected > 0) {
      // todo error handling
      this->_activeTiles.insert({entity.posX(), entity.posY()});
    }

    // add in map
    currentTile->entities.insert(&entity);
  }
}

World::World(int width, int height)
  : _width{width}
  , _height(height)
  , _map{
    std::vector<std::vector<Tile>>(
      width,
      std::vector<Tile>(height, {0})
    )
  }
{}

World::World(int width, int height, std::vector<Entity> &entities)
  : World(width, height)
{
  this->entities = std::move(entities); //todo controlla sta roba

  // move removes entities from original vector.
  // this must use this->.
  // Initialize map with entities
  for (auto &entity : this->entities) {
    this->_entityPostLoop(entity);
  }
}

World::World(int width, int height, int entityCount)
  : World(width, height)
{
  this->entities.reserve(entityCount);
  for (int i = 0; i < entityCount; i++) {
    this->entities.emplace_back(i, 0, 0, AI::randomAi);
  }
}

bool World::isInside(const Entity &entity) const {
  const int posX = entity.posX();
  const int posY = entity.posY();

  return posX < this->_width && posX >= 0 && posY < this->_height && posY >= 0;
}

void World::loop() {
  ++(this->_minutesPassed);
  for (auto &entity : this->entities) {
    this->_entityPreLoop(entity);

    entity.loop();

    this->_entityPostLoop(entity);
  }

  // For each entity in tile, spread infection. Note that active tiles are the
  // One where at least one infected person exists, for now.
  for (auto &tile : this->_activeTiles) {
    for (auto &entityPtr : this->_map[tile.x][tile.y].entities) {
      entityPtr->daysInfected = 1;
    }
  }

  // Execute next day logic (Must be last)
  if (this->_minutesPassed >= MINUTES_IN_A_DAY) {
    return this->_nextDay();
  }
}

void World::_nextDay() {
  ++(this->_daysPassed);
  this->_minutesPassed = 0;

  printf("New day!\n");

  for (auto &entity : this->entities) {
    if (entity.daysInfected > 14) {
      entity.daysInfected = 0;
    } else if (entity.daysInfected > 0) {
      entity.daysInfected++;
    }
  }
}

