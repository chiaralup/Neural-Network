#include <SFML/Graphics.hpp>
#include <iostream>

#include "hopfield.hpp"

// ALLA FINE RICORDIAMOCI DI VERIFICARE CHE NON CI SIANO LIBRERIE NON USATE E
// CAPIRE SE VANNO NELL'HPP O NEL CPP

// int main() {
//   try {
//     nn::Hopfield hop(42, 51);
//
//     std::cout << "Choose an image: Avogadro, Curie, Einstein,"
//                  "Heisenberg, Hopfield or Schrodinger\n";
//     std::string name;
//     std::cin >> name;
//
//     std::string filename = name + ".png";
//
//     // auto patterns{hop.loadPatterns()};
//     // hop.matrix(patterns);
//
//     hop.getMatrix();
//
//     nn::Matrix W{hop.getMatrix()};
//
//     // Display dis{hop.screen(filename)};
//
//     nn::Drawable initial{hop.loadSprite(filename)};
//     initial.sprite.setPosition(70., 180.);
//     nn::Pattern baw_pattern{hop.pattern(initial.image)};
//     nn::Drawable blackandwhite{hop.baw_image(baw_pattern)};
//     blackandwhite.sprite.setScale(3.f, 3.f);
//     blackandwhite.sprite.setPosition(325., 250.);
//     nn::Pattern corr_pattern{hop.corruption(baw_pattern, 10)};
//     nn::Drawable corrupted{hop.baw_image(corr_pattern)};
//     corrupted.sprite.setScale(3.f, 3.f);
//     corrupted.sprite.setPosition(475., 250.);
//
//     bool first_screen{true};
//     sf::Clock clock;
//
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Neural Network");
//
//     while (window.isOpen()) {
//       sf::Event event;
//       while (window.pollEvent(event)) {
//         if (event.type == sf::Event::Closed) {
//           window.close();
//         }
//
//         if (event.type == sf::Event::KeyPressed &&
//             event.key.code == sf::Keyboard::Enter) {
//           first_screen = false;
//         }
//       }
//
//       window.clear();
//
//       if (first_screen) {
//         window.draw(initial.sprite);
//         window.draw(blackandwhite.sprite);
//         window.draw(corrupted.sprite);
//
//       } else {
//         auto evolution{hop.updating(corr_pattern, W)};
//
//         for (nn::Pattern p : evolution) {
//           nn::Drawable updated{hop.baw_image(p)};
//           updated.sprite.setScale(3.f, 3.f);
//           updated.sprite.setPosition(325., 100.);
//           window.draw(updated.sprite);
//
//           std::cout << "Energy = " << hop.energy(p) << '\n';
//         }
//
//         clock.restart();
//       }
//       window.draw(initial.sprite);
//       window.draw(blackandwhite.sprite);
//       window.draw(corrupted.sprite);
//
//       window.display();
//     }
//
//   }
//   //  Display display{hop.display(filename)};
//   //  auto pattern{hop.pattern(display.blackandwhite.image)};}
//   catch (const std::exception& e) {
//     std::cerr << "Exception: " << e.what() << '\n';
//     return EXIT_FAILURE;
//   } catch (...) {
//     std::cerr << "Unknown exception occurred!" << '\n';
//     return EXIT_FAILURE;
//   }
// }

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
    nn::Drawable corrupted{hop.baw_image(corr_pattern)};
    corrupted.sprite.setScale(3.f, 3.f);
    corrupted.sprite.setPosition(450., 250.);

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

      nn::Drawable updated{hop.baw_image(curr_state)};
      updated.sprite.setScale(3.f, 3.f);
      updated.sprite.setPosition(650., 100.);

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

// int main() {
//   try {
//     nn::Hopfield hop(42, 51);
//
//     std::cout << "Choose an image: Avogadro, Curie, Einstein,"
//                  " Heisenberg, Hopfield or Schrodinger\n";
//     std::string name;
//     std::cin >> name;
//
//     std::string filename = name + ".png";
//
//     // auto patterns{hop.loadPatterns()};
//     // hop.matrix(patterns);
//
//     hop.getMatrix();
//
//     nn::Matrix W{hop.getMatrix()};
//
//     // Display dis{hop.screen(filename)};
//
//     nn::Drawable initial{hop.loadSprite(filename)};
//     initial.sprite.setPosition(175., 180.);
//     nn::Pattern baw_pattern{hop.pattern(initial.image)};
//     nn::Drawable blackandwhite{hop.baw_image(baw_pattern)};
//     blackandwhite.sprite.setScale(3.f, 3.f);
//     blackandwhite.sprite.setPosition(350., 180.);
//     nn::Pattern corr_pattern{hop.corruption(baw_pattern, 10)};
//     nn::Drawable corrupted{hop.baw_image(corr_pattern)};
//     corrupted.sprite.setScale(3.f, 3.f);
//     corrupted.sprite.setPosition(425., 180.);
//
//     auto evolution{hop.update(corr_pattern)};
//     std::cout << "Evolution size: " << evolution.size() << '\n';
//     size_t step{0};
//
//     // bool first_screen{true};
//
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Neural Network");
//     window.setFramerateLimit(60);
//
//     int screenState{0};
//     sf::Clock clock;
//     const sf::Time updateInterval{sf::seconds(1.f)};
//
//     while (window.isOpen()) {
//       sf::Event event;
//       while (window.pollEvent(event)) {
//         if (event.type == sf::Event::Closed) {
//           window.close();
//         }
//
//         if (event.type == sf::Event::KeyPressed &&
//             event.key.code == sf::Keyboard::Enter) {
//           if (screenState < 2) {
//             ++screenState;
//           }
//           clock.restart();
//           // first_screen = false;
//         }
//       }
//
//       window.clear();
//       switch (screenState) {
//         case 0:
//           window.draw(initial.sprite);
//           window.draw(blackandwhite.sprite);
//           break;
//         case 1:
//           window.draw(corrupted.sprite);
//           break;
//         case 2:
//           if (step < evolution.size()) {
//             nn::Drawable updated{hop.baw_image(evolution[step])};
//             updated.sprite.setScale(3.f, 3.f);
//             updated.sprite.setPosition(350., 225.);
//             window.draw(updated.sprite);
//           }
//
//           if (clock.getElapsedTime() >=
//               updateInterval) {  // DA RIMETTERE L'ENERGIA
//             if (step < evolution.size()) {
//               ++step;  // vai al pattern successivo
//             }
//             clock.restart();
//           }
//
//           break;
//           // nn::Pattern next_state{hop.update(current_evolution_state,
//           W)};
//           // if (next_state != current_evolution_state) {
//           //   current_evolution_state = next_state;
//           //   sf::Image
//           //   new_image{hop.baw_image(current_evolution_state).image};
//           //   updated_drawable.texture.loadFromImage(new_image);
//           // updated_drawable.sprite.setTexture(updated_drawable.texture);
//           //   updated_drawable.sprite.setScale(3.f, 3.f);
//           //   updated_drawable.sprite.setPosition(350., 225.);
//           //  std::cout << "Energy = " <<
//           //  hop.energy(current_evolution_state)
//           //            << '\n';
//           //}
//           // clock.restart();
//           // break;
//       }
//       window.display();
//     }
//
//   }
//
//   //  Display display{hop.display(filename)};
//   //  auto pattern{hop.pattern(display.blackandwhite.image)};}
//   catch (const std::exception& e) {
//     std::cerr << "Exception: " << e.what() << '\n';
//     return EXIT_FAILURE;
//   } catch (...) {
//     std::cerr << "Unknown exception occurred!" << '\n';
//     return EXIT_FAILURE;
//   }
// }
//