#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "config.hpp"
#include "world.hpp"

class Engine {
  sf::RenderWindow window_;
  sf::Texture backgroundTexture_;
  sf::Sprite backgroundSprite_;

  // Holds shapes for all entities to be drawn
  std::vector<sf::CircleShape> SFMLEntities_;

  // Hold game world and config.
  Config config_{};

  World world_;

 public:
  Engine(std::string backgroundFilePath, std::string entitiesFilePath);
};

#endif // define ENGINE_HPP
