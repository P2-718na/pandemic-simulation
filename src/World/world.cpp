#include "world.hpp"

#include <utility>

////////////////MOVE
#include <random>
std::random_device rd{};
std::mt19937 gen{rd()};
int randint (int min, int max) {
  std::normal_distribution<> d{((double)(max + min) / 2),100};
  return d(gen);
}

void rand(Entity* _this) {

  int x, y;
  x = randint(0, 800);
  y = randint(0, 800);

  _this->moveTo(x, y);
}
/////////////////////

void World::_entityPreLoop(Entity &entity) {
  // get tile the entity is in pre-loop
  Tile* currentTile = &this->_map[entity.posX()][entity.posY()];

  // decrease its value
  --(currentTile->value);

  // remove from map
  currentTile->entities.erase(entity.uid());
}

void World::_entityPostLoop(Entity &entity) {
  // get tile the entity should be in post-loop
  Tile* currentTile = &this->_map[entity.posX()][entity.posY()];

  // increase its value
  ++(currentTile->value);

  // add in map
  currentTile->entities.insert({entity.uid(), &entity});
}

World::World(int width, int height)
  : _map{
    std::vector<std::vector<Tile>>(
      width,
      std::vector<Tile>(height, {0})
    )
  }
{}

World::World(int width, int height, std::vector<Entity> entities)
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
    //todo check warning
    this->entities.push_back(Entity{i, 0, 0, rand});
  }
}

void World::loop() {
  for (auto &entity : this->entities) {
    // FIXME this causes segfault
    //this->_entityPreLoop(entity);

    entity.loop();

    // FIXME this causes segfault
    // this->_entityPostLoop(entity);
  }
}

