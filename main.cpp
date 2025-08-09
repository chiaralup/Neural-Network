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

      Display display{hop.display(filename)};

      window.clear();
      window.draw(display.initial.sprite);
      window.draw(display.blackandwhite.sprite);
      window.draw(display.corrupted.sprite);
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