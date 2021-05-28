#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "config.hpp"
#include "world.hpp"
#include "entity.hpp"

class Engine {
  sf::RenderWindow window_{sf::VideoMode{800, 800}, "Simulation"};
  sf::Texture backgroundTexture_;

  // SFML documentation states that Sprite is initialized by taking a texture
  // reference. However, if I pass the texture to the sprite constructor
  // before the texture is initialized, the sprite doesn't update its texture.
  // This means that this has to be initialized in constructor.
  sf::Sprite backgroundSprite_;

  sf::Clock clock_{};

  // Holds shapes for all entities to be drawn.
  std::vector<sf::CircleShape> SFMLEntities_;

  // Hold simulation world and config.
  Config config_{};
  World world_;

  // RenderWindow max refresh rate in fps. Does not affect simulation speed,
  // but having a lower refresh rate makes the program run faster, since
  // less time is spent in drawing the image.
  int refreshRate_{240};

  // Draw a blue shade when it's night time.
  bool daylightCycleEnabled{true};

  // Handle all sfml events
  void handleEvents_(sf::Event const& event) noexcept;

  // Display the simulation. Gets called based on refresh rate.
  void graphicsLoop_() noexcept;

  // Cycle refresh rate. The values are fixed: (in fps)
  // 4, 60, 240, -1 (unlimited)
  void cycleRefreshRate_() noexcept;

  // Return color of an entity to display.
  // Note that we can't return const reference, because the default
  // color is not a sf::Color constant.
  static sf::Color getEntityColour_(Entity const& entity) noexcept;

 public:
  // Constructor ///////////////////////////////////////////////////////////////
  Engine(std::string const& backgroundPath, std::string const& entitiesPath);

  // Methods ///////////////////////////////////////////////////////////////////
  // Start simulation, run till stopped.
  void startSimulation() noexcept;

  // Print a message to the console.
  // It can be upgraded by printing a message to the simulation window instead.
  static void printMessage(std::string const& message) noexcept;
};

#endif // define ENGINE_HPP
