#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics/Image.hpp>

#include "iworld.hpp"
#include "entity.hpp"

// todo config file
#define MINUTES_IN_A_DAY 9000

enum day {
  monday,
  tuesday,
  wednesday,
  thursday,
  friday,
  saturday,
  sunday
};

struct Tile {
  float entityCount{};
  std::vector<Entity*> entities;
};

class World : IWorld{
  int _width;
  int _height;
  int _daysPassed{0};
  int _minutesPassed{0};

  sf::Image _background;
  std::vector<std::vector<Tile>> _map;
  std::vector<Tile*> _activeTiles;

  std::vector<std::pair<int, int>> _walkCoords;
  std::vector<std::pair<int, int>> _shopCoords;
  std::vector<std::pair<int, int>> _partyCoords;

  // Todo there should be some error management here
  // Loops /////////////////////////////////////////////////////////////////////
  void _entityPreLoop(Entity &entity);
  void _entityPostLoop(Entity &entity);
  void _dayLoop();

  // Private methods ///////////////////////////////////////////////////////////
  void _initMap();
  void _initEntities(std::vector<Entity> &entities);
  void _parseImage();

 public:
  // todo decide if this should be public or private
  std::vector<Entity> entities;

  // Constructors //////////////////////////////////////////////////////////////
  //World();
  World(int width, int height);
  World(int width, int height, std::vector<Entity> &entities);
  World(int width, int height, int entityCount); //this is gonna be removed most likely

  // These two will be used. Maybe some init function would be great.
  World(const std::string &backgroundImagePath, std::vector<Entity> &entities);
  World(const std::string &backgroundImagePath, const std::string &entitiesFile);


  // Accessors /////////////////////////////////////////////////////////////////
  int time() const final;
  sf::Image background();

  // todo implement variation for range (maybe just on random walk?)
  std::pair<int, int> randomWalkCoords() final;
  std::pair<int, int> randomShopCoords() final;
  std::pair<int, int> randomPartyCoords() final;

  // Methods ///////////////////////////////////////////////////////////////////
  void loop();
  bool isInside(const Entity &entity) const;
  day weekDay() const;

  // Static ////////////////////////////////////////////////////////////////////
  static bool parseEntities(
    const std::string &entitiesFile,
    std::vector<Entity> &entities
  );
};