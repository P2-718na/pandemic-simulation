#include <cmath>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>
#include <stdexcept>

#include "engine.hpp"
#include "entity.hpp"

namespace pandemic {

// Constructor /////////////////////////////////////////////////////////////////
Engine::Engine(
  std::string const& backgroundPath, std::string const& entitiesPath)
  : world_{backgroundPath, entitiesPath, config_} {
  if (!backgroundTexture_.loadFromImage(world_.background())) {
    throw std::runtime_error("Cannot load background texture.");
  }

  // Set video mode based on background size
  const sf::VideoMode videoMode{
    backgroundTexture_.getSize().x, backgroundTexture_.getSize().y};
  window_.create(videoMode, "Simulation");

  // SFML documentation states that Sprite is initialized by taking a texture
  // reference. However, if I pass the texture to the sprite constructor
  // before the texture is initialized, the sprite doesn't update its texture.
  // This means that this has to be initialized in constructor.
  backgroundSprite_.setTexture(backgroundTexture_);

  // we must do this here instead of initialization list, because world must be
  // initialized before
  SFMLEntities_ =
    std::vector<sf::CircleShape>(world_.entityCount(), sf::CircleShape{1.6, 4});
}

// Methods /////////////////////////////////////////////////////////////////////
void Engine::startSimulation() noexcept {
  while (window_.isOpen()) {
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

    // Print data for current day every day loop
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
    if (event.key.code == sf::Keyboard::D) {
      cycleRefreshRate_();
      printMessage("New refresh rate: " + std::to_string(refreshRate_));
      return;
    }
    if (event.key.code == sf::Keyboard::L) {
      world_.lockdown(!world_.lockdown());
      const std::string message =
        world_.lockdown() ? "Lockdown enabled!" : "Lockdown disabled!";
      printMessage(message);
      return;
    }
    if (event.key.code == sf::Keyboard::K) {
      daylightCycleEnabled ^= true;
      const std::string message = daylightCycleEnabled
                                  ? "Daylight cycle enabled!"
                                  : "Daylight cycle disabled!";
      printMessage(message);
      return;
    }
  }
}

void Engine::handleDayLoop_() noexcept {
  const int newInfected = world_.infectedCount() - lastDayInfectedCount_;

  std::stringstream ss;
  ss << "New day!" << std::endl
     << "Total infected: " << world_.infectedCount()
     << ", New infected: " << newInfected
     << ", Dead: " << world_.deadCount()
     << ", Immune: " << world_.immuneCount();

  printMessage(ss.str());

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

  // Loop through every entity and set its respective sf::CircleShape
  // color and position according to its state.
  assert(world_.entityCount() == SFMLEntities_.size());
  for (int i = 0; i != world_.entityCount(); ++i) {
    const Entity& entity = world_.entity(i);
    sf::CircleShape& SFMLEntity = SFMLEntities_[i];

    SFMLEntity.setPosition(
      static_cast<float>(entity.posX()), static_cast<float>(entity.posY()));
    SFMLEntity.setFillColor(getEntityColour_(entity));

    window_.draw(SFMLEntity);
  }

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

sf::Color Engine::getEntityColour_(Entity const& entity) noexcept {
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
  return {0xaa, 0x00, 0xff};
}

float Engine::currentLightLevel_() noexcept {
  const float b = static_cast<float>(config_.MINUTES_IN_A_DAY()) / 2.f;
  const float a = M_PI / b;
  const auto x = static_cast<float>(world_.currentMinute());

  // Cosine function, transformed so that it is between 0 and 1.
  // It has a peak for x=b, and period T=2b
  return static_cast<float>((std::cos(a * (x - b)) + 1.f)) / 2.f;
}

void Engine::tintBackground_() noexcept {
  // Do some math to calculate tint color.
  // We want to colour the sprites from 0xaa,0xaa,0xff to 0xff,0xff,0xff
  // 0x55 is the range (0xaa-0xff)
  const sf::Uint8 tint = currentLightLevel_() * (0x55) + 0xaa;
  assert(tint <= 0xff);
  assert(tint >= 0xaa);
  backgroundSprite_.setColor({tint, tint, 0xff});
}

}  // namespace pandemic
