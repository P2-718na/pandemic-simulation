#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "engine.hpp"
#include "entity.hpp"

namespace pandemic {

// Constructor /////////////////////////////////////////////////////////////////
Engine::Engine(
  const std::string& backgroundPath, const std::string& entitiesPath)
  : world_{ backgroundPath, entitiesPath, config_ } {
  // Try to load background image.
  if (!backgroundTexture_.loadFromImage(world_.background())) {
    throw std::runtime_error("Cannot load background texture.");
  }

  // Set video mode based on background size
  const sf::VideoMode videoMode{ backgroundTexture_.getSize().x,
                                 backgroundTexture_.getSize().y };
  window_.create(videoMode, "Simulation");

  // SFML documentation states that Sprite is initialized by taking a texture
  // reference. However, if I pass the texture to the sprite constructor
  // before the texture is initialized, the sprite doesn't update its texture.
  // This means that this has to be initialized here.
  backgroundSprite_.setTexture(backgroundTexture_);

  // Initialize SFML drawables vector after world has already been initialised.
  SFMLEntities_ = std::vector<sf::CircleShape>(
    world_.entityCount(),
    sf::CircleShape{ 1.6, 4 });
}

// Methods /////////////////////////////////////////////////////////////////////
void Engine::startSimulation() noexcept {
  while (window_.isOpen()) {
    // Handle events
    sf::Event event{};
    while (window_.pollEvent(event)) {
      handleEvent_(event);
    }

    // Draw graphics according to refresh rate
    const sf::Time elapsed = clock_.getElapsedTime();
    if (elapsed.asSeconds() >= 1.f / static_cast<float>(refreshRate_)) {
      graphicsLoop_();
      clock_.restart();
    }

    // Print data of current day every day loop
    if (world_.currentMinute() == 0) {
      handleDayLoop_();
    }

    // Loop simulation world
    world_.loop();
  }
}

void Engine::printMessage(const std::string& message) noexcept {
  std::cout << message << std::endl;
}

void Engine::handleEvent_(const sf::Event& event) noexcept {
  if (event.type == sf::Event::Closed) {
    // Close window. This will end the loop and close simulation.
    window_.close();
    return;
  }

  // Handle key presses
  if (event.type == sf::Event::KeyReleased) {
    switch (event.key.code) {
      case sf::Keyboard::D:
        cycleRefreshRate_();
        printMessage("New refresh rate: " + std::to_string(refreshRate_));
        break;

      case sf::Keyboard::L:
        world_.lockdown(!world_.lockdown());
        if (world_.lockdown()) {
          printMessage("Lockdown enabled!");
          break;
        }

        printMessage("Lockdown disabled!");
        break;

      case sf::Keyboard::K:
        // Flip daylightCycleEnabled
        daylightCycleEnabled ^= true;

        if (daylightCycleEnabled) {
          printMessage("Daylight cycle enabled!");
          break;
        }

        printMessage("Daylight cycle disabled!");
        break;

      default:
        break;
    }
  }
}

void Engine::handleDayLoop_() noexcept {
  // Generate string with infection-related data...
  std::stringstream ss;
  ss << "New day!" << std::endl
     << "Total infected: " << world_.infectedCount()
     << ", New infected: " << world_.infectedCount() - lastDayInfectedCount_
     << ", Dead: " << world_.deadCount()
     << ", Immune: " << world_.immuneCount();

  // and print it.
  printMessage(ss.str());

  // Update lastDayInfectedCount_ to be used in next day loop.
  lastDayInfectedCount_ = world_.infectedCount();
}

void Engine::graphicsLoop_() noexcept {
  // This is not strictly needed, since we will be redrawing the background
  // which takes up the whole screen, but it's recommended by sfml.
  window_.clear();

  // Tint background based on time of day, if daylightCycle is enabled
  if (daylightCycleEnabled) {
    tintBackground_();
  }

  // Draw background image
  window_.draw(backgroundSprite_);

  assert(world_.entityCount() == SFMLEntities_.size());
  // Loop through every entity and set its respective sf::CircleShape
  // color and position according to its state.
  for (int i = 0; i != world_.entityCount(); ++i) {
    const Entity& entity = world_.entity(i);
    sf::CircleShape& SFMLEntity = SFMLEntities_[i];

    SFMLEntity.setPosition(
      static_cast<float>(entity.posX()),
      static_cast<float>(entity.posY()));
    SFMLEntity.setFillColor(getEntityColour_(entity));

    // Draw this entity.
    window_.draw(SFMLEntity);
  }

  // Display window.
  window_.display();
}

void Engine::cycleRefreshRate_() noexcept {
  switch (refreshRate_) {
    case 4:
      refreshRate_ = 60;
      break;
    case 60:
      refreshRate_ = 240;
      break;
    case 240:
      refreshRate_ = -1;
      break;
    default:
      refreshRate_ = 4;
      break;
  }
}

sf::Color Engine::getEntityColour_(const Entity& entity) noexcept {
  // These values are hard-coded here, since "config" class is for parameters
  // that are strictly related to the simulation.
  if (entity.dead()) {
    return sf::Color::Black;
  }
  if (entity.infected()) {
    return sf::Color::Red;
  }
  if (entity.immune()) {
    return sf::Color::Cyan;
  }

  // Default color is purple.
  return { 0xaa, 0x00, 0xff };
}

float Engine::currentLightLevel_() noexcept {
  // These are parameters to transform a cosine function to our needs.
  // We want to get values in [0; 1], we want it to be 1 when it's
  // midday and we want its period to be exactly a day.

  // b is translation along x-axis.
  const float b = static_cast<float>(config_.MINUTES_IN_A_DAY()) / 2.f;

  // a is used to change the period.
  const float a = M_PI / b;

  // x is the independent variable of the function (the current minute of the
  // day).
  const auto x = static_cast<float>(world_.currentMinute());

  // We can now build our cosine function.
  return static_cast<float>((std::cos(a * (x - b)) + 1.f)) / 2.f;
}

void Engine::tintBackground_() noexcept {
  // Do some math to calculate tint color.
  // We want to colour the sprites from 0xaa,0xaa,0xff to 0xff,0xff,0xff
  // 0x55 is the range (0xaa-0xff)
  const sf::Uint8 tint = currentLightLevel_() * (0x55) + 0xaa;

  assert(tint <= 0xff);
  assert(tint >= 0xaa);

  backgroundSprite_.setColor({ tint, tint, 0xff });
}

}  // namespace pandemic
