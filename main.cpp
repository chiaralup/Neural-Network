#include <SFML/Graphics.hpp>
#include <iostream>

#include "hopfield.hpp"

// ALLA FINE RICORDIAMOCI DI VERIFICARE CHE NON CI SIANO LIBRERIE NON USATE E
// CAPIRE SE VANNO NELL'HPP O NEL CPP

int main() {
  try {
    nn::Hopfield hop(42, 51);

    std::cout << "Choose an image: Avogadro, Curie, Einstein,"
                 "Heisenberg, Hopfield or Schrodinger\n";
    std::string name;
    std::cin >> name;

    std::string filename = name + ".png";

    auto patterns{hop.loadPatterns()};
    hop.matrix(patterns);

    hop.getMatrix();

    nn::Matrix W{hop.getMatrix()};

    // Display dis{hop.screen(filename)};

    nn::Drawable initial{hop.loadSprite(filename)};
    initial.sprite.setPosition(70., 180.);
    nn::Pattern baw_pattern{hop.pattern(initial.image)};
    nn::Drawable blackandwhite{hop.baw_image(baw_pattern)};
    blackandwhite.sprite.setScale(3.f, 3.f);
    blackandwhite.sprite.setPosition(325., 250.);
    nn::Pattern corr_pattern{hop.corruption(baw_pattern, 10)};
    nn::Drawable corrupted{hop.baw_image(corr_pattern)};
    corrupted.sprite.setScale(3.f, 3.f);
    corrupted.sprite.setPosition(475., 250.);

    bool first_screen{true};
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Neural Network");

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Enter) {
          first_screen = false;
        }
      }

      window.clear();

      if (first_screen) {
        window.draw(initial.sprite);
        window.draw(blackandwhite.sprite);
        window.draw(corrupted.sprite);

      } else {
        auto evolution{hop.updating(corr_pattern, W)};

        for (nn::Pattern p : evolution) {
          nn::Drawable updated{hop.baw_image(p)};
          updated.sprite.setScale(3.f, 3.f);
          updated.sprite.setPosition(325., 100.);
          window.draw(updated.sprite);

          std::cout << "Energy = " << hop.energy(p) << '\n';
        }

        clock.restart();
      }
      window.draw(initial.sprite);
      window.draw(blackandwhite.sprite);
      window.draw(corrupted.sprite);
    }
    window.display();
  }
  //  Display display{hop.display(filename)};
  //  auto pattern{hop.pattern(display.blackandwhite.image)};}
  catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown exception occurred!" << '\n';
    return EXIT_FAILURE;
  }
}