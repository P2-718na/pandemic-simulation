#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "config.hpp"
#include "world.hpp"
#include "entity.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "covid");
  sf::Texture backgroundTexture;
  sf::Sprite background;

  std::vector<sf::CircleShape> SFMLEntities(1000, sf::CircleShape{1.6, 4});

  Config config{};
  // Unneeded?
  //config.initRandomGenerator();

  World world("background.sample.bmp", "entities", config);

  backgroundTexture.loadFromImage(world.background());
  background.setTexture(backgroundTexture);
  //background.setScale(2, 2);

  bool draw = true;
  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::D) {
        draw = !draw;
      }
    }

    //window.clear();
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
