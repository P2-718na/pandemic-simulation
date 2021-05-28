#include <vector>
#include <iostream>
#include <cassert>
#include <stdexcept>

#include "engine.hpp"
#include "entity.hpp"

// Constructor /////////////////////////////////////////////////////////////////
Engine::Engine(
  std::string const& backgroundPath, std::string const& entitiesPath)
  : world_{backgroundPath, entitiesPath, config_} {
  if (!backgroundTexture_.loadFromImage(world_.background())) {
   throw std::runtime_error("Cannot load background texture.");
  }

  backgroundSprite_.setTexture(backgroundTexture_);

  // we must do this here instead of initialization list, because world must be
  // initialized before
  SFMLEntities_ = std::vector<sf::CircleShape>(
    world_.entityCount(), sf::CircleShape{1.6, 4});
}

// Methods /////////////////////////////////////////////////////////////////////
void Engine::startSimulation() noexcept {
  while (window_.isOpen()) {
    sf::Event event{};
    while (window_.pollEvent(event)) {
      handleEvents_(event);
    }

    // Draw graphics according to refresh rate
    const sf::Time elapsed = clock_.getElapsedTime();
    if (elapsed.asSeconds() >= 1.f / static_cast<float>(refreshRate_)) {
      graphicsLoop_();
      clock_.restart();
    }

    // Loop simulation world
    world_.loop();
  }
}

void Engine::printMessage(const std::string& message) noexcept {
  std::cout << message << std::endl;
}

void Engine::handleEvents_(const sf::Event& event) noexcept {
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
      const std::string message = world_.lockdown()
                                  ? "Lockdown enabled!"
                                  : "Lockdown disabled!";
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

void Engine::graphicsLoop_() noexcept {
  window_.clear();

  //todo refactor
  if (daylightCycleEnabled) {
    if (world_.currentMinute() > config_.hourToMinutes(21) || world_.currentMinute() < config_.hourToMinutes(6)) {
      backgroundSprite_.setColor({0xaa, 0xaa, 0xff});
    } else {
      backgroundSprite_.setColor({0xff, 0xff, 0xff});
    }
  }

  window_.draw(backgroundSprite_);

  // Loop through every entity and set its respective sf::CircleShape
  // color and position according to its state.
  assert(world_.entityCount() == SFMLEntities_.size());
  for (int i = 0; i != world_.entityCount(); ++i) {
    const Entity& entity = world_.entity(i);
    sf::CircleShape& SFMLEntity = SFMLEntities_[i];

    SFMLEntity.setPosition(
      static_cast<float>(entity.posX()),
      static_cast<float>(entity.posY()));

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
  if (entity.infectionResistance >= .99f) {
    return sf::Color::Cyan;
  }
  return {0xaa, 0x00, 0xff};
}
