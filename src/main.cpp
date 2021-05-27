#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "config.hpp"
#include "world.hpp"
#include "entity.hpp"
#include "engine.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "covid");
  sf::Texture backgroundTexture;
  sf::Sprite background;

  std::vector<sf::CircleShape> SFMLEntities(1000, sf::CircleShape{1.6, 4});

  Config config{};
  World world("background.sample.bmp", "entities", config);

  backgroundTexture.loadFromImage(world.background());
  background.setTexture(backgroundTexture);
  //background.setScale(2, 2);
  bool draw = true;
  bool daylightCycle = false;
  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::D) {
          draw = !draw;
          continue;
        }

        if (event.key.code == sf::Keyboard::L) {
          world.lockdown(!world.lockdown());
        }

        if (event.key.code == sf::Keyboard::K) {
          daylightCycle = !daylightCycle;
        }
      }
    }

    //window.clear();
    if (daylightCycle) {
      if (world.currentMinute() > config.hourToMinutes(21) || world.currentMinute() < config.hourToMinutes(6)) {
        background.setColor({0xaa, 0xaa, 0xff});
      } else {
        background.setColor({0xff, 0xff, 0xff});
      }
    }

    window.draw(background);
    world.loop();

    for (int i = 0; i < 1000; i++) {
      auto e = &world.entities()[i];
      //printf("entity %d is at pos x:%d, y:%d", i, e->posX(), e->posY());
      SFMLEntities[i].setPosition(e->posX(), e->posY());
      if (e->dead()) {
        SFMLEntities[i].setFillColor(sf::Color::Black);
      } else if (e->infected()) {
        SFMLEntities[i].setFillColor(sf::Color::Red);
      } else if (e->infectionResistance >= .99f) {
        SFMLEntities[i].setFillColor(sf::Color::Cyan);
      } else {
        SFMLEntities[i].setFillColor({0xaa, 0x00, 0xff});
      }
    }

    if (draw) {
      for (auto & entity : SFMLEntities) {
        window.draw(entity);
      }
      window.display();
    }
  }


  return 0;
}
