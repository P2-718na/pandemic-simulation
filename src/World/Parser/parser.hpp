#include <SFML/Graphics/Image.hpp>
#include <string>
#include <utility>
#include <vector>

#include "config.hpp"
#include "Entity/AI/ai.hpp"

class Entity;
class World;

typedef std::pair<int, int> Coords;

struct Parser {
  static bool parsePointsOfInterests(const Config& config,
    const sf::Image& backgroundImage, std::vector<Coords>& parkCoords,
    std::vector<Coords>& shopCoords, std::vector<Coords>& partyCoords) noexcept;

  static bool parseEntitiesFile(World* parentPtr,
    const std::string& entitiesFile, std::vector<Entity>& entities);

  static entityAI parseEntityAI(const std::string & AIName);
};
