#include <iostream>
#include <SFML/Graphics.hpp>

#include "bitmap.hpp"
#include "pathfinder.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "covid");
  sf::Texture img;
  sf::Sprite sprite;

  img.loadFromFile("./test.bmp");
  sprite.setTexture(img);
  sprite.setScale(2, 2);

  window.draw(sprite);
  window.display();

  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }


  }
  return 0;
}
