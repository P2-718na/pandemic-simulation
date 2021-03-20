#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#include "entity.hpp"
#include "pathfinder.hpp"
#include "world.hpp"
#include "ai.hpp"

// values near the mean are the most likely
// standard deviation affects the dispersion of generated values from the mean

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "covid");
  sf::Texture backgroundTexture;
  sf::Sprite background;

  backgroundTexture.loadFromFile("./test.bmp");
  background.setTexture(backgroundTexture);
  background.setScale(2, 2);

  sf::VertexArray _entities(sf::Points, 1000);

  std::vector<Entity> _e;
  _e.reserve(1000);
  for (int i = 0; i < 1000; i++)   {
    _e.emplace_back(i, AI::randInt(100, 700), AI::randInt(100, 700), AI::randomAi);
  }
  for (int i = 0; i < 100; i++) {
    _e[i].tryInfect();
  }

  World world(800, 800, _e);
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
      auto e = &world.entities[i];
      //printf("entity %d is at pos x:%d, y:%d", i, e->posX(), e->posY());
      _entities[i].position = sf::Vector2f(e->posX(), e->posY());
      _entities[i].color = e->infective() ? sf::Color::Red : sf::Color::White;
    }

    if (draw) {
      window.draw(_entities);
      window.display();
    }
  }


  return 0;
}
