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

typedef std::vector<Tile> Column;
typedef std::vector<Column> Map;

class World : IWorld{
  int width_;
  int height_;
  int currentDay_{0};
  int currentMinute_{0};

  sf::Image backgroundImage_;
  Map map_;
  std::vector<Tile*> activeTiles_;

  std::vector<Coords> parkCoords_;
  std::vector<Coords> shopCoords_;
  std::vector<Coords> partyCoords_;

  // Todo there should be some error management here
  // Loops /////////////////////////////////////////////////////////////////////
  void _entityPreLoop(Entity &entity);
  void _entityPostLoop(Entity &entity);
  void _dayLoop();

  // Private methods ///////////////////////////////////////////////////////////
  void initMap_();
  void parseBackground_();

  bool parseEntitiesFromFile_(
    const std::string &entitiesFile,
    std::vector<Entity> &entities
  );

 public:
  // todo decide if this should be public or private
  std::vector<Entity> entities;

  // Constructors //////////////////////////////////////////////////////////////
  World(const std::string &backgroundImagePath, const std::string &entitiesFile);


  // Accessors /////////////////////////////////////////////////////////////////
  int time() const final;
  sf::Image background();

  Coords randomParkCoords() final;
  Coords randomShopCoords() final;
  Coords randomPartyCoords() final;

  // Methods ///////////////////////////////////////////////////////////////////
  void loop();
  bool isInside(const Entity &entity) const;
  day weekDay() const;

  // Static ////////////////////////////////////////////////////////////////////
  static entityAi parseEntityAi(const std::string& value);
};
