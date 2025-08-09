#include <SFML/Graphics.hpp>
#include <iostream>

#include "hopfield.hpp"

// ALLA FINE RICORDIAMOCI DI VERIFICARE CHE NON CI SIANO LIBRERIE NON USATE E
// CAPIRE SE VANNO NELL'HPP O NEL CPP

int main() {
  try {
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Neural Network");

    Hopfield hop;

    std::cout << "Choose an image: Pillars.jpg or Earring.png" << '\n';
    std::string filename;
    std::cin >> filename;

    while (window.isOpen()) {
      sf::Event event;

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
      }

      Drawable image{hop.loadSprite(filename)};
      image.sprite.setPosition(25., 250.);

      Drawable blackandwhite{hop.blackandwhite(image.image)};
      blackandwhite.sprite.setPosition(650., 250.);

      // Drawable corrupted{hop.corruption(image.image)};
      // corrupted.sprite.setPosition(1150., 250.);

      window.clear();
      window.draw(image.sprite);
      window.draw(blackandwhite.sprite);
      // window.draw(corrupted.sprite);
      window.display();
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown exception occurred!" << '\n';
    return EXIT_FAILURE;
  }
};