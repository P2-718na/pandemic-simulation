#include <iostream>
#include <SFML/Graphics.hpp>

#include "bitmap.hpp"
#include "entity.hpp"
#include "pathfinder.hpp"

void ciao(Entity* _) {
  std::cout << "ciao" << std::endl;
}

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "covid");
  sf::Texture img;
  sf::Sprite sprite;

  img.loadFromFile("./test.bmp");
  sprite.setTexture(img);
  sprite.setScale(2, 2);

  window.draw(sprite);
  window.display();

  Entity entity{10, 10, ciao};
  entity.moveTo(300, 300);

  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    std::cout << "1";
    entity.loop();
  }
  return 0;
}
