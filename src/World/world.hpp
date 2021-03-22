#pragma once
#include <vector>

#include "iworld.hpp"
#include "entity.hpp"

// todo config file
#define MINUTES_IN_A_DAY 1440

struct Tile {
  float entityCount{};
  std::vector<Entity*> entities;
};

class World : IWorld{
  int _width;
  int _height;
  int _daysPassed{0};
  int _minutesPassed{0};

  std::vector<std::vector<Tile>> _map;
  std::vector<Tile*> _activeTiles;

  // Todo there should be some error management here
  // Loops /////////////////////////////////////////////////////////////////////
  void _entityPreLoop(Entity &entity);
  void _entityPostLoop(Entity &entity);
  void _dayLoop();

 public:
  // todo decide if this should be public or private
  std::vector<Entity> entities;

  // Constructors //////////////////////////////////////////////////////////////
  //World();
  World(int width, int height);
  World(int width, int height, std::vector<Entity> &entities);
  World(int width, int height, int entityCount); //this is gonna be removed most likely

  // Accessors /////////////////////////////////////////////////////////////////
  int time() const final;

  // Methods ///////////////////////////////////////////////////////////////////
  bool isInside(const Entity &entity) const;
  void loop();
};