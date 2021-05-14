#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics/Image.hpp>

#include "iworld.hpp"
#include "entity.hpp"

// todo config file
constexpr int MINUTES_IN_A_DAY = 9000;

struct Tile {
  float entityCount{};
  std::vector<Entity*> entities;
};

class World : IWorld{
  int width_;
  int height_;
  int currentDay_{0};
  int currentMinute_{0};

  sf::Image backgroundImage_;

  std::vector<Coords> parkCoords_;
  std::vector<Coords> shopCoords_;
  std::vector<Coords> partyCoords_;

  // Loops /////////////////////////////////////////////////////////////////////
  void dayLoop_();
  void spreadVirus_();

  // Private methods ///////////////////////////////////////////////////////////
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
  Day currentDay() const final;
  int currentMinute() const final;

  const sf::Image& background();

  const Coords& randomParkCoords() final;
  const Coords& randomShopCoords() final;
  const Coords& randomPartyCoords() final;

  int infectedCount() const;

  // Methods ///////////////////////////////////////////////////////////////////
  void loop();

  // Static ////////////////////////////////////////////////////////////////////
  static entityAi parseEntityAi(const std::string& value);
};
