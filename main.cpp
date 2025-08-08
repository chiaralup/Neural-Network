#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>

// ALLA FINE RICORDIAMOCI DI VERIFICARE CHE NON CI SIANO LIBRERIE NON USATE E
// CAPIRE SE VANNO NELL'HPP O NEL CPP

int main() {
  try {
    sf::RenderWindow window(sf::VideoMode(640, 640), "Neural Network");

    // std::cout << "Choose an image: 1 or 2" << '\n';
    // int image;
    // std::cin >> image;

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