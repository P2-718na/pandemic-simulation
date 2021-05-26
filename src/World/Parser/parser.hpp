#include <SFML/Graphics/Image.hpp>
#include <string>
#include <vector>

#include "types.hpp"
#include "config.hpp"
#include "Entity/AI/ai.hpp"

class Entity;
class World;

struct Parser {
  // returns false if one of the three lists is empty.
  static bool parsePointsOfInterests(const Config& config,
    const sf::Image& backgroundImage, std::vector<Coords>& parkCoords,
    std::vector<Coords>& shopCoords, std::vector<Coords>& partyCoords) noexcept;

  static bool parseEntitiesFile(World* parentPtr,
    const std::string& entitiesFile, std::vector<Entity>& entities);

  static entityAI parseEntityAI(const std::string & AIName);
};
