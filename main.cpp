#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
  sf::Image image1;
  sf::Image image2;
  sf::Texture texture1;
  sf::Texture texture2;

  sf::Vector2u pillars{image1.getSize()};
  sf::Vector2u deep{image2.getSize()};

  double bx{pillars.x / deep.x};
  double by{pillars.y / deep.y};

  for (int x = 0; x < pillars.x; x++) {
  }

  sf::RenderWindow window(sf::VideoMode(800, 600), "Neural Network");

  if (!image1.loadFromFile("pillars.jpg")) {
    std::cerr << "Errore nel caricamento dell'immagine" << '\n';
    return -1;
  }

  if (!image2.loadFromFile("deepsky.jpg")) {
    std::cerr << "Errore nel caricamento dell'immagine" << '\n';
    return -1;
  }

  if (!texture1.loadFromFile("pillars.jpg")) {
    return -1;
  }

  if (!texture2.loadFromFile("deepsky.jpg")) {
    return -1;
  }

  std::cout << image1.getSize().x << " " << image1.getSize().y << '\n';
  std::cout << image2.getSize().x << " " << image2.getSize().y << '\n';

  // sf::Sprite sprite1;
  // sprite1.setTexture(texture1);
  // sf::Sprite sprite2;
  // sprite2.setTexture(texture2);
  //
  // while (window.isOpen()) {
  //   sf::Event event;
  //   while (window.pollEvent(event)) {
  //     if (event.type == sf::Event::Closed) window.close();
  //   }
  //
  //   window.clear();
  //   window.display();
  // }
  //
  // return 0;
}
