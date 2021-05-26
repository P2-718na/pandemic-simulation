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

  sf::VertexArray _entities(sf::Points, 1000);

  Config config{};

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
      _entities[i].position = sf::Vector2f(e->posX(), e->posY());
      if (e->dead()) {
        _entities[i].color = sf::Color::Black;
      } else if (e->infected()) {
        _entities[i].color = sf::Color::Red;
      } else {
        _entities[i].color = sf::Color::Blue;
      }
    }

    if (draw) {
      window.draw(_entities);
      window.display();
    }
  }


  return 0;
}
