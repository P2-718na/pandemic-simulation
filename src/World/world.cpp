#include "world.hpp"
#include "ai.hpp"

#include <utility>

void World::_entityPreLoop(Entity &entity) {
  // If entity is in a valid position, update board
  if (isInside(entity)) {
    // get tile the entity is in pre-loop
    Tile *currentTile = &this->_map[entity.posX()][entity.posY()];

    // decrease its value
    --(currentTile->value);

    // remove from map
    currentTile->entities.erase(entity.uid());
  }
}

void World::_entityPostLoop(Entity &entity) {
  // If entity is in a valid position, update board
  if (isInside(entity)) {
    // get tile the entity should be in post-loop
    Tile *currentTile = &this->_map[entity.posX()][entity.posY()];

    // increase its value
    ++(currentTile->value);

    // add in map
    currentTile->entities.insert({entity.uid(), &entity});
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
  for (auto &entity : this->entities) {
    this->_entityPreLoop(entity);

    entity.loop();

    this->_entityPostLoop(entity);
  }

  if (this->_minutesPassed >= MINUTES_IN_A_DAY) {
    return this->_nextDay();
  }
}

void World::_nextDay() {
  ++(this->_daysPassed);
  this->_minutesPassed = 0;

  for (auto &entity : this->entities) {
    if (entity.daysInfected > 14) {
      entity.daysInfected == 0;
    } else if (entity.daysInfected > 0) {
      entity.daysInfected++;
    }
  }
}

