#include <SFML/Graphics.hpp>
#include <iostream>

#include "hopfield.hpp"

// ALLA FINE RICORDIAMOCI DI VERIFICARE CHE NON CI SIANO LIBRERIE NON USATE E
// CAPIRE SE VANNO NELL'HPP O NEL CPP

int main() {
  try {
    Hopfield hop;
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Neural Network");

    // std::cout << "Choose an image: Pillars.jpg or Earring.png" << '\n';
    std::string filename{"Earring.png"};
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
    corrupted.sprite.setScale(3.f, 3.f);
    corrupted.sprite.setPosition(1150., 250.);

    std::vector<int> current_state{corr_pattern};
    std::vector<int> next_state{current_state};
    bool finished{false};
    sf::Clock clock;

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
      }

      if (!finished && clock.getElapsedTime().asMilliseconds() > 500) {
        next_state = hop.up(current_state);
        if (next_state == current_state) {
          finished = true;
        }
        current_state = next_state;
        std::cout << "Energy = " << hop.energy(current_state) << '\n';
        clock.restart();
      }

      window.clear();
      window.draw(corrupted.sprite);

      Drawable updated{hop.blackandwhite(current_state)};
      updated.sprite.setScale(3.f, 3.f);
      updated.sprite.setPosition(100., 100.);
      window.draw(updated.sprite);
      window.display();

      // window.draw(initial);
      // window.draw(blackandwhite);
      // window.draw(corrupted);
      // window.draw(initial.sprite);
      // window.draw(blackandwhite.sprite);
    }
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