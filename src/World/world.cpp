#include "world.hpp"
#include "ai.hpp"

// Loops ///////////////////////////////////////////////////////////////////////
void World::_entityPreLoop(Entity &entity) {
  // If entity is in a valid position, clear it from the board
  if (isInside(entity)) {
    // get tile the entity is in pre-loop
    Tile *currentTile = &this->_map[entity.posX()][entity.posY()];

    // remove from map (do not clear same tile twice).
    if (!currentTile->entities.empty()) {
      currentTile->entities.clear();
      currentTile->entityCount = 0;
    }
  }
}

void World::_entityPostLoop(Entity &entity) {
  // If entity is in a valid position, update board
  if (isInside(entity)) {
    // get tile the entity should be in post-loop
    Tile *currentTile = &this->_map[entity.posX()][entity.posY()];

    // increase its entity count  and add to active tiles
    // Count tile with only one entity as inactive, for now.
    // Also note that a tile must succeed a spread check in order to make a tile
    // active.
    ++(currentTile->entityCount);
    if (
      currentTile->entityCount > 1 && entity.infective() &&
      AI::chanceCheck(entity.baseSpreadChance())
    ) {
      this->_activeTiles.insert(&_map[entity.posX()][entity.posY()]);
    }

    // add in map
    currentTile->entities.insert(&entity);
  }
}

void World::_dayLoop() {
  ++(this->_daysPassed);
  this->_minutesPassed = 0;

  int contagiati = 0;
  for (auto &entity : this->entities) {
    entity.dayLoop();

    if (entity.infective()) {
      contagiati++;
    }
  }

  printf("New day! %d\n", this->_daysPassed);
  printf("Sani: %d, Contagiati: %d\n", this->entities.size() - contagiati, contagiati);
}

void World::loop() {
  ++(this->_minutesPassed);

  // Empty _activeTiles, since they will be regenerated.
  this->_activeTiles.clear();

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
  for (auto &tilePtr : this->_activeTiles) {
    for (auto &entityPtr : tilePtr->entities) {
      entityPtr->tryInfect();
    }
  }

  // Execute next day logic (Must be called last)
  if (this->_minutesPassed >= MINUTES_IN_A_DAY) {
    return this->_dayLoop();
  }
}


// Constructors ////////////////////////////////////////////////////////////////
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

// Methods /////////////////////////////////////////////////////////////////////
bool World::isInside(const Entity &entity) const {
  const int posX = entity.posX();
  const int posY = entity.posY();

  return posX < this->_width && posX >= 0 && posY < this->_height && posY >= 0;
}