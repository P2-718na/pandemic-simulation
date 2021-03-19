#pragma once
#include <vector>
#include <unordered_map>

#include "entity.hpp"

struct Tile {
  float value{};
  std::unordered_map<int, Entity*> entities;
};

class World {
  // per ora questo contiene semplicemente il livello di coviddi
  int _width;
  int _height;
  std::vector<std::vector<Tile>> _map;

  // Todo there should be some error management here
  void _entityPreLoop(Entity &entity);
  void _entityPostLoop(Entity &entity);

 public:
  // todo decide if this should be public or private
  std::vector<Entity> entities;
  //World();
  World(int width, int height);
  World(int width, int height, std::vector<Entity> entities);
  World(int width, int height, int entityCount);

  bool isInside(const Entity &entity) const;

  void loop();
};