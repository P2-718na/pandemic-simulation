#ifndef PARSER_HPP
#define PARSER_HPP

#include <SFML/Graphics/Image.hpp>
#include <string>
#include <vector>

#include "types.hpp"

class Entity;
class World;
class Config;

struct Parser {
  // returns false if one of the three lists is empty.
  static bool parsePointsOfInterests(const Config& config,
    const sf::Image& backgroundImage, std::vector<Coords>& parkCoords,
    std::vector<Coords>& shopCoords, std::vector<Coords>& partyCoords) noexcept;

  static bool parseEntitiesFile(World* parentPtr,
    const std::string& entitiesFile, std::vector<Entity>& entities);
};

#endif // define PARSER_HPP
