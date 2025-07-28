#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Neural Network");

  sf::Image image1;
  if (!image1.loadFromFile("pillars.jpg")) {
    std::cerr << "Errore nel caricamento dell'immagine" << '\n';
    return -1;
  }
  sf::Image image2;
  if (!image2.loadFromFile("merginggalaxies.jpg")) {
    std::cerr << "Errore nel caricamento dell'immagine" << '\n';
    return -1;
  }

  sf::Texture texture1;
  if (!texture1.loadFromFile("pillars.jpg")) {
    return -1;
  }
  sf::Texture texture2;
  if (!texture2.loadFromFile("merginggalaxies.jpg")) {
    return -1;
  }

  std::cout << image1.getSize().x << " " << image1.getSize().y << '\n';
  std::cout << image2.getSize().x << " " << image2.getSize().y << '\n';

  //sf::Sprite sprite1;
  //sprite1.setTexture(texture1);
  //sf::Sprite sprite2;
  //sprite2.setTexture(texture2);
//
  //while (window.isOpen()) {
  //  sf::Event event;
  //  while (window.pollEvent(event)) {
  //    if (event.type == sf::Event::Closed) window.close();
  //  }
//
  //  window.clear();
  //  window.display();
  //}

  return 0;
}
