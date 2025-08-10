#include <SFML/Graphics.hpp>
#include <iostream>

#include "hopfield.hpp"

// ALLA FINE RICORDIAMOCI DI VERIFICARE CHE NON CI SIANO LIBRERIE NON USATE E
// CAPIRE SE VANNO NELL'HPP O NEL CPP

int main() {
  try {
    Hopfield hop;
    sf::RenderWindow& window{hop.window()};

    // std::cout << "Choose an image: Pillars.jpg or Earring.png" << '\n';
    std::string filename{"Pillars.jpg"};
    // std::cin >> filename;

    // hop.matrix();

    hop.getMatrix();

    // Display dis{hop.screen(filename)};

    Drawable initial{hop.loadSprite(filename)};
    initial.sprite.setPosition(25., 250.);
    std::vector<int> baw_pattern{hop.pattern(initial.image)};
    Drawable blackandwhite{hop.blackandwhite(baw_pattern)};
    blackandwhite.sprite.setPosition(650., 250.);
    std::vector<int> corr_pattern{hop.corruption(baw_pattern)};
    Drawable corrupted{hop.blackandwhite(corr_pattern)};
    corrupted.sprite.setPosition(1150., 250.);

    // std::ifstream file{"./weights.txt"};
    // if (!file) {
    //   throw std::runtime_error{"Impossible to open file!"};
    // }
    //
    //    for (int i{0}; i < 10; ++i) {
    //      double Wij{0.};
    //      file >> Wij;
    //      std::cout << Wij << '\n';
    //    }
    //

    // while (window.isOpen()) {
    //   sf::Event event;
    //   while (window.pollEvent(event)) {
    //     if (event.type == sf::Event::Closed) {
    //       window.close();
    //     }
    //   }
    //
    //  window.clear();
    //  hop.update(corr_pattern);

    // window.draw(initial);
    // window.draw(blackandwhite);
    // window.draw(corrupted);
    // window.draw(initial.sprite);
    //  window.draw(blackandwhite.sprite);
    //   window.draw(corrupted.sprite);
    //   window.display();
    // }
    // std::cout << "Valori bianchi in pattern2: "
    //            << std::count(upd_pattern.begin(), upd_pattern.end(), 1) <<
    //            '\n';
    //  Display display{hop.display(filename)};
    //  auto pattern{hop.pattern(display.blackandwhite.image)};
    //  std::cout << "Valori bianchi in pattern2: "
    //            << std::count(pattern.begin(), pattern.end(), 1) << '\n';
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown exception occurred!" << '\n';
    return EXIT_FAILURE;
  }
};