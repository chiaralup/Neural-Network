#include <SFML/Graphics.hpp>
#include <iostream>

#include "hopfield.hpp"

// ALLA FINE RICORDIAMOCI DI VERIFICARE CHE NON CI SIANO LIBRERIE NON USATE E
// CAPIRE SE VANNO NELL'HPP O NEL CPP

int main() {
  try {
    // sf::RenderWindow window(sf::VideoMode(1600, 900), "Neural Network");

    Hopfield hop;

    // std::cout << "Choose an image: Pillars.jpg or Earring.png" << '\n';
    std::string filename{"Pillars.jpg"};
    // std::cin >> filename;

    hop.matrix();

    // Display dis{hop.screen(filename)};

    // Matrix m{hop.matrix()};

    // Drawable initial{hop.loadSprite(filename)};
    // initial.sprite.setPosition(25., 250.);
    //
    // std::vector<int> baw_pattern{hop.pattern(initial.image)};
    // Drawable blackandwhite{hop.blackandwhite(baw_pattern)};
    // blackandwhite.sprite.setPosition(650., 250.);
    //
    // std::vector<int> corr_pattern{hop.corruption(baw_pattern)};
    // Drawable corrupted{hop.blackandwhite(corr_pattern)};
    // corrupted.sprite.setPosition(1150., 250.);
    //
    // Drawable updated{corrupted};
    // updated.sprite.setPosition(0., 0.);
    //
    // while (window.isOpen()) {
    //  sf::Event event;
    //  while (window.pollEvent(event)) {
    //    if (event.type == sf::Event::Closed) {
    //      window.close();
    //    }
    //  }
    //  //  // hop.update(corr_pattern, m);
    //  //
    //  window.clear();
    //  //  // window.draw(initial);
    //  //  // window.draw(blackandwhite);
    //  //  // window.draw(corrupted);
    //  //
    //  //  // window.draw(initial.sprite);
    //  //  // window.draw(blackandwhite.sprite);
    //  //  // window.draw(corrupted.sprite);
    //  //
    //  //  //while (hop.update(corr_pattern, m) == false) {
    //  window.draw(updated.sprite);
    //  //  //}
    //  window.display();
    //}

    // std::cout << "Valori bianchi in pattern2: "
    //           << std::count(upd_pattern.begin(), upd_pattern.end(), 1) <<
    //           '\n';
    // Display display{hop.display(filename)};
    // auto pattern{hop.pattern(display.blackandwhite.image)};
    // std::cout << "Valori bianchi in pattern2: "
    //           << std::count(pattern.begin(), pattern.end(), 1) << '\n';

  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown exception occurred!" << '\n';
    return EXIT_FAILURE;
  }
};