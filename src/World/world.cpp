#include <iostream>

#include "world.hpp"
#include "ai.hpp"

// Loops ///////////////////////////////////////////////////////////////////////
void World::_entityPreLoop(Entity &entity) {
  // If entity is in a valid position, clear it from the board
  if (isInside(entity)) {
    // get tile the entity is in pre-loop
    Tile *currentTile = &this->_map[entity.posX()][entity.posY()];

    // remove from map.
    currentTile->entities.clear();
    currentTile->entityCount = 0;
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
      AI::chanceCheck(entity.virusSpreadChance())
    ) {
      this->_activeTiles.push_back(&_map[entity.posX()][entity.posY()]);
    }

    // add in map
    currentTile->entities.push_back(&entity);
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
  printf("Sani: %ld, Contagiati: %d\n", this->entities.size() - contagiati, contagiati);
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

// Private methods /////////////////////////////////////////////////////////////
void World::_initMap() {
  this->_map =
    std::vector<std::vector<Tile>>(
      this->_width,
      std::vector<Tile>(this->_height, {0})
    );
}

void World::_initEntities(std::vector<Entity> &entities) {
  // Original entities vector will be cleared.
  this->entities = std::move(entities);

  // Initialize map with entities. Note that this-> is required here.
  for (auto &entity : this->entities) {
    entity.setParent(this);
  }
}

void World::_parseImage() {
  for (int y = 0; y < this->_height; ++y) {
    for (int x = 0; x < this->_width; ++x ) {
      sf::Color color = this->_background.getPixel(x, y);

      // Todo actual parsing
      // sample, for now
      if (color.r == 0x0 && color.g == 0xff && color.b == 0x0) {
        this->_walkCoords.emplace_back(x, y);
      } else if (color.r == 0xff && color.g == 0xFF && color.b == 0x0) {
        this->_shopCoords.emplace_back(x, y);
      } else if (color.r == 0xff && color.g == 0x0 && color.b == 0xff) {
        this->_partyCoords.emplace_back(x, y);
      }
    }
  }
}

// Constructors ////////////////////////////////////////////////////////////////
World::World(int width, int height)
  : _width{width}
  , _height(height)
{
  this->_initMap();
}

World::World(int width, int height, std::vector<Entity> &entities)
  : World(width, height)
{
  this->_initEntities(entities);
}

World::World(int width, int height, int entityCount)
  : World(width, height)
{
  this->entities.reserve(entityCount);
  for (int i = 0; i < entityCount; i++) {
    this->entities.emplace_back(i, 0, 0, AI::randomAi);
  }
}

World::World(
  const std::string &backgroundImagePath,
  std::vector<Entity> &entities
) {
  if (!this->_background.loadFromFile(backgroundImagePath)) {
    throw std::runtime_error("Cannot load image");
  };

  this->_width = this->_background.getSize().x;
  this->_height = this->_background.getSize().y;
  this->_initMap();
  this->_initEntities(entities);
  this->_parseImage();
}

// Accessors ///////////////////////////////////////////////////////////////////
int World::time() const {
  return this->_minutesPassed;
}

sf::Image World::background() {
  return this->_background;
}

std::pair<int, int> World::randomWalkCoords() {
  return {500, 500}; //todo
}
std::pair<int, int> World::randomShopCoords() {
  return {500, 500}; //todo
}
std::pair<int, int> World::randomPartyCoords() {
  return {500, 500}; //todo
}

// Methods /////////////////////////////////////////////////////////////////////
bool World::isInside(const Entity &entity) const {
  const int posX = entity.posX();
  const int posY = entity.posY();

  return posX < this->_width && posX >= 0 && posY < this->_height && posY >= 0;
}