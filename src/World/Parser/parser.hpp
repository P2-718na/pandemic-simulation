#ifndef PANDEMIC_PARSER_HPP
#define PANDEMIC_PARSER_HPP

#include <SFML/Graphics/Image.hpp>
#include <string>
#include <vector>

#include "types.hpp"
#include "world.hpp"

namespace pandemic {

class Entity;
class Config;

// Parser is a class that provides methods for parsing background and entities
// files.
struct World::Parser {
  // Parse points of interest from background image.
  // throws if one of the three lists is empty.
  static void parsePointsOfInterests(
    const Config& config, const sf::Image& backgroundImage,
    std::vector<Coords>& parkCoords, std::vector<Coords>& shopCoords,
    std::vector<Coords>& partyCoords);

  // Parse entities file.
  // Can throw if there is any error parsing entities file
  static void parseEntitiesFile(
    World* parentPtr, const std::string& entitiesFile,
    std::vector<Entity>& entities);
};

}  // namespace pandemic

#endif  // define PANDEMIC_PARSER_HPP
