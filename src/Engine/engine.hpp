#ifndef PANDEMIC_ENGINE_HPP
#define PANDEMIC_ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "config.hpp"
#include "entity.hpp"
#include "world.hpp"

namespace pandemic {

class Engine {
  // SFML-related members
  sf::RenderWindow window_;
  sf::Texture backgroundTexture_;
  sf::Sprite backgroundSprite_;

  // Each entity in the simulation is associated to a corresponding SFML shape.
  std::vector<sf::CircleShape> SFMLEntities_;

  // Hold simulation world and config.
  Config config_{};
  World world_;

  // Clock used to draw the screen at specified intervals.
  // (See refreshRate_ below)
  sf::Clock clock_{};

  // RenderWindow max refresh rate in fps. Does not affect simulation speed,
  // but having a lower refresh rate makes the program run faster, since
  // less time is spent in drawing the image.
  int refreshRate_{ -1 };

  // Draw a blue shade when it's night time.
  bool daylightCycleEnabled{ true };

  // Number of infected people after the last time World::dayLoop() was called.
  int lastDayInfectedCount_{ 0 };

  // Handle all sfml events.
  void handleEvent_(const sf::Event& event) noexcept;

  // Instructions that need to be run every day loop.
  void handleDayLoop_() noexcept;

  // Display the simulation. Gets called based on refresh rate.
  void graphicsLoop_() noexcept;

  // Cycle refresh rate. The values are fixed: (in fps)
  // 4, 60, 240, -1 (unlimited).
  void cycleRefreshRate_() noexcept;

  // Return color of an entity to display.
  // Note that we can't return const reference, because the default
  // color is not a sf::Color constant.
  static sf::Color getEntityColour_(const Entity& entity) noexcept;

  // Return a float from 0 to 1 which represents the current light level.
  float currentLightLevel_() noexcept;

  // Tint background sprite based on time of day. Gets called only if
  // daylightCycle is enabled.
  void tintBackground_() noexcept;

 public:
  // Constructor ///////////////////////////////////////////////////////////////
  Engine(const std::string& backgroundPath, const std::string& entitiesPath);

  // Methods ///////////////////////////////////////////////////////////////////
  // Start simulation.
  void startSimulation() noexcept;

  // Print a message to the console.
  // It can be upgraded by printing a message to the simulation window instead.
  static void printMessage(const std::string& message) noexcept;
};

}  // namespace pandemic

#endif  // define PANDEMIC_ENGINE_HPP
