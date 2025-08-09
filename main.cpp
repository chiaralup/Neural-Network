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

      Drawable image{hop.loadImage(filename)};
      image.sprite.setPosition(0., 0.);

      std::vector<int> pattern{hop.pattern(image.image)};
      Drawable blackandwhite{hop.blackandwhite(pattern)};
      blackandwhite.sprite.setPosition(400., 0.);
      //
      // Drawable corrupted{hop.blackandwhite(hop.corruption(pattern))};
      // corrupted.sprite.setPosition(800., 0.);

      window.clear();
      // window.draw(image.sprite);
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