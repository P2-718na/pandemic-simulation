#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

#include "bitmap.hpp"
#include "entity.hpp"
#include "pathfinder.hpp"

std::random_device rd{};
std::mt19937 gen{rd()};

// values near the mean are the most likely
// standard deviation affects the dispersion of generated values from the mean


int randint (int min, int max) {
  std::normal_distribution<> d{((double)(max + min) / 2),100};
  return d(gen);
}

void rand(Entity* _this) {

  int x, y;
  x = randint(0, 800);
  y = randint(0, 800);

  _this->moveTo(x, y);
}

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "covid");
  sf::Texture backgroundTexture;
  sf::Sprite background;

  backgroundTexture.loadFromFile("./test.bmp");
  background.setTexture(backgroundTexture);
  background.setScale(2, 2);

  std::vector<Entity> entities(2000, {10, 10, rand});
  sf::VertexArray _entities(sf::Points, 2000);

  for (int i = 0; i < 2000; i++) {
    _entities[i].color = sf::Color::Red;
  }

  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    //window.clear();
    window.draw(background);

    for (int i = 0; i < 2000; i++) {
      auto e = &entities[i];
      e->loop();
      _entities[i].position = sf::Vector2f(e->posX(), e->posY());
    }

    window.draw(_entities);

    window.display();
  }


  return 0;
}
