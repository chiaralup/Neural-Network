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
    initial.sprite.setPosition(25., 250.);
    nn::Pattern pattern{hop.pattern(initial.image)};
    nn::Drawable blackandwhite{hop.baw_image(pattern)};
    blackandwhite.sprite.setPosition(250., 250.);
    blackandwhite.sprite.setScale(3.f, 3.f);

    nn::Pattern corr_pattern{hop.corruption(pattern, 10)};
    std::vector<nn::Pattern> evolution{hop.update(corr_pattern)};
    std::vector<nn::Drawable> transitions;
    for (auto const& p : evolution) {
      nn::Drawable transition{hop.baw_image(p)};
      transition.sprite.setScale(3.f, 3.f);
      transition.sprite.setPosition(650., 250.);
      transitions.push_back(transition);
    }

    std::cout << "Evolution size: " << evolution.size() << '\n';

    nn::Drawable corrupted{hop.baw_image(corr_pattern)};
    corrupted.sprite.setPosition(475., 250.);
    corrupted.sprite.setScale(3.f, 3.f);

    unsigned step{0};
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Neural Network");

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
      }

      // if (clock.getElapsedTime().asSeconds() >= 1.f &&
      //     step < evolution.size() - 1) {
      //   transition = hop.baw_image(evolution[step]);
      //   transition.sprite.setScale(3.f, 3.f);
      //   transition.sprite.setPosition(650., 250.);
      //   std::cout << "Step " << step
      //             << "-> Energy = " << hop.energy(evolution[step]) << '\n';
      //   clock.restart();
      //
      //  ++step;
      //
      //} else if (step == evolution.size() - 1) {
      //  std::cout << "Step " << step
      //            << " -> Energy = " << hop.energy(evolution[step]) << '\n';
      //}

      window.clear();
      window.draw(initial.sprite);
      window.draw(blackandwhite.sprite);
      window.draw(corrupted.sprite);
      window.draw(transitions[0].sprite);
      window.display();
    }
  } catch (std::exception const& e) {
    std::cerr << "Caught exception: '" << e.what() << "'\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}
