#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>

int main() {
  try {
    sf::RenderWindow window(sf::VideoMode(640, 640), "Neurale Network");
    sf::Texture texture;

    sf::Sprite sprite;
    sprite.setTexture(texture);

    while (window.isOpen()) {
      sf::Event event;

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
        window.clear();
        window.display();
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown exception occurred!" << '\n';
    return EXIT_FAILURE;
  }
};