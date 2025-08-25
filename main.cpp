#include <SFML/Graphics.hpp>
#include <iostream>

#include "hopfield.hpp"

int main() {
  try {
    nn::Hopfield hop(42, 51);

    if (!std::filesystem::exists("weights.txt")) {
      hop.matrix(hop.loadPatterns());
    } else {
      hop.getMatrix();
    }

    std::cout
        << "Choose the image you would like to try and right down the name: "
           "Avogadro, Curie, Einstein, Heisenber, Hopfield or Schrodinger\n";
    std::string filename;
    std::cin >> filename;
    filename += ".png";

    nn::Drawable initial{hop.loadSprite(filename)};
    initial.sprite.setPosition(25., 225.);
    nn::Pattern pattern{hop.pattern(initial.image)};
    nn::Drawable blackandwhite{hop.bawImage(pattern)};
    blackandwhite.sprite.setPosition(250., 225.);
    blackandwhite.sprite.setScale(3.f, 3.f);

    nn::Pattern corr_pattern{hop.corruption(pattern, 10)};
    nn::Drawable corrupted{hop.bawImage(corr_pattern)};
    corrupted.sprite.setScale(3.f, 3.f);
    corrupted.sprite.setPosition(450., 225.);

    nn::Pattern prev_state{corr_pattern};
    nn::Pattern curr_state{prev_state};
    bool finished{false};
    int step{0};

    sf::Clock clock;

    std::vector<double> energies;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Neural Network");

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
      }

      window.clear();

      if (!finished && clock.getElapsedTime().asMilliseconds() > 500 &&
          step < 10) {
        curr_state = hop.update(prev_state);

        if (curr_state == prev_state) {
          finished = true;
        }

        energies.push_back(hop.energy(prev_state));

        prev_state = curr_state;

        clock.restart();
      }

      nn::Drawable updated{hop.bawImage(curr_state)};
      updated.sprite.setScale(3.f, 3.f);
      updated.sprite.setPosition(650., 225.);

      window.draw(initial.sprite);
      window.draw(blackandwhite.sprite);
      window.draw(corrupted.sprite);
      window.draw(updated.sprite);
      window.display();
    }

    std::cout << "Energies during the updates:\n";
    for (auto e : energies) {
      std::cout << e << ' ';
    }
    std::cout << '\n';

  } catch (std::exception const& e) {
    std::cerr << "Caught exception: '" << e.what() << "'\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}