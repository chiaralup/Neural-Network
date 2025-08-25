#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "hopfield.hpp"

TEST_CASE("Testing loadImage and loadSprite functions") {
  nn::Hopfield hop(42, 51);

  REQUIRE(hop.getWidth() == 42);   //??? o check ???
  REQUIRE(hop.getHeight() == 51);  //??? o check ???
  REQUIRE(hop.getN() == 42 * 51);  //??? o check ???

  SUBCASE("Loading a valid image") {
    nn::Drawable drawable{hop.loadSprite("Avogadro.png")};
    CHECK(drawable.image.getSize().x > 0);
    CHECK(drawable.image.getSize().y > 0);
    CHECK(drawable.texture.getSize().x > 0);
    CHECK(drawable.texture.getSize().y > 0);
    CHECK(drawable.sprite.getTexture() != nullptr);
  }

  SUBCASE("Loading an invalid image") {
    CHECK_THROWS(hop.loadImage("Pauli.png"));   // hanno sensoo??
    CHECK_THROWS(hop.loadSprite("Pauli.png"));  // hanno sensoo??
  }
}

TEST_CASE("Testing Hopfield Neural Network") {
  nn::Hopfield hop(2, 2);
  SUBCASE("Testing a total black image") {
    sf::Image total_black;
    total_black.create(3, 3, sf::Color::Black);
    auto resized{hop.resize_image(total_black)};

    CHECK(resized.size() == hop.getN());

    for (unsigned i{0}; i < hop.getN(); ++i) {
      CHECK(resized[i].r == 0);
      CHECK(resized[i].g == 0);
      CHECK(resized[i].b == 0);
    }

    nn::Pattern black_pattern{hop.pattern(total_black)};
    std::vector<int> v(hop.getN(), -1);

    CHECK(black_pattern == v);

    nn::Drawable black_drawable{hop.baw_image(black_pattern)};

    CHECK(black_drawable.image.getSize().x == hop.getWidth());
    CHECK(black_drawable.image.getSize().y == hop.getHeight());

    for (unsigned i{0}; i < hop.getWidth(); ++i) {
      for (unsigned j{0}; j < hop.getHeight(); ++j) {
        sf::Color color = black_drawable.image.getPixel(i, j);
        CHECK(color == sf::Color::Black);
      }
    }

    CHECK(black_drawable.texture.getSize().x == hop.getWidth());
    CHECK(black_drawable.texture.getSize().y == hop.getHeight());
    CHECK(black_drawable.sprite.getTexture() != nullptr);
  }

  SUBCASE("Testing a total white image") {
    sf::Image total_white;
    total_white.create(3, 3, sf::Color::White);
    auto resized{hop.resize_image(total_white)};

    CHECK(resized.size() == hop.getN());

    for (unsigned i{0}; i < hop.getN(); ++i) {
      CHECK(resized[i].r == 255);
      CHECK(resized[i].g == 255);
      CHECK(resized[i].b == 255);
    }

    nn::Pattern white_pattern{hop.pattern(total_white)};
    std::vector<int> v(hop.getN(), 1);

    CHECK(white_pattern == v);

    nn::Drawable white_drawable{hop.baw_image(white_pattern)};

    CHECK(white_drawable.image.getSize().x == hop.getWidth());
    CHECK(white_drawable.image.getSize().y == hop.getHeight());

    for (unsigned i{0}; i < hop.getWidth(); ++i) {
      for (unsigned j{0}; j < hop.getHeight(); ++j) {
        sf::Color color{white_drawable.image.getPixel(i, j)};
        CHECK(color == sf::Color::White);
      }
    }

    CHECK(white_drawable.texture.getSize().x == hop.getWidth());
    CHECK(white_drawable.texture.getSize().y == hop.getHeight());
    CHECK(white_drawable.sprite.getTexture() != nullptr);
  }

  SUBCASE("Testing a 3x3 image") {
    sf::Image image;
    image.create(3, 3, sf::Color::Black);
    image.setPixel(0, 0, sf::Color(200, 127, 150));
    image.setPixel(0, 1, sf::Color(28, 34, 160));
    image.setPixel(1, 0, sf::Color(63, 164, 102));
    image.setPixel(1, 1, sf::Color(150, 87, 255));
    image.setPixel(2, 0, sf::Color(221, 126, 60));
    image.setPixel(2, 2, sf::Color(255, 155, 0));

    auto resized{hop.resize_image(image)};
    CHECK(resized.size() == hop.getN());

    CHECK(resized[0].r == 200);
    CHECK(resized[0].g == 127);
    CHECK(resized[0].b == 150);
    CHECK(resized[1].r == 142);
    CHECK(resized[1].g == 145);
    CHECK(resized[1].b == 81);
    CHECK(resized[2].r == 14);
    CHECK(resized[2].g == 17);
    CHECK(resized[2].b == 80);
    CHECK(resized[3].r == 101);
    CHECK(resized[3].g == 61);
    CHECK(resized[3].b == 64);

    nn::Pattern pattern{hop.pattern(image)};
    std::vector<int> v = {1, -1, -1, -1};

    CHECK(pattern == v);

    nn::Drawable drawable{hop.baw_image(pattern)};

    CHECK(drawable.image.getSize().x == hop.getWidth());
    CHECK(drawable.image.getSize().y == hop.getHeight());

    CHECK(drawable.image.getPixel(0, 0) == sf::Color::White);
    CHECK(drawable.image.getPixel(1, 0) == sf::Color::Black);
    CHECK(drawable.image.getPixel(0, 1) == sf::Color::Black);
    CHECK(drawable.image.getPixel(1, 1) == sf::Color::Black);

    CHECK(drawable.texture.getSize().x == hop.getWidth());
    CHECK(drawable.texture.getSize().y == hop.getHeight());
    CHECK(drawable.sprite.getTexture() != nullptr);
  }
}

TEST_CASE("Testing loadPatterns") {
  nn::Hopfield hop(42, 51);

  std::vector<std::string> files = {"Avogadro.png", "Curie.png",
                                    "Einstein.png", "Heisenberg.png",
                                    "Hopfield.png", "Schrodinger.png"};

  SUBCASE("Should load the correct number of patterns and correct size") {
    std::vector<nn::Pattern> loadedPatterns = hop.loadPatterns();

    CHECK(loadedPatterns.size() ==
          files.size());  // pattern caricati = numero file

    for (const auto& pat : loadedPatterns) {
      CHECK(pat.size() == hop.getN());  // dimensione giusta
    }

    if (!loadedPatterns.empty()) {  // controllo siano -1 o 1
      for (const auto& pat : loadedPatterns) {
        CHECK(!pat.empty());
        for (int val : pat) {
          CHECK((val == -1 || val == 1));
        }
      }
    }
  }
}

// NON SO SE SIANO NECESSARIE, DA VEDERE SE TOGLIERLE O LASCIARLE

TEST_CASE("Testing the function baw_image") {
  SUBCASE("Testing a 3x3 image") {
    nn::Hopfield hop(3, 3);

    sf::Image img;
    img.create(3, 3, sf::Color::Black);
    img.setPixel(0, 0, sf::Color(200, 127, 150));
    img.setPixel(1, 1, sf::Color(150, 87, 255));
    img.setPixel(2, 0, sf::Color(168, 59, 207));

    auto pat{hop.pattern(img)};

    nn::Drawable drawable{hop.baw_image(pat)};

    CHECK(drawable.image.getSize().x == hop.getWidth());
    CHECK(drawable.image.getSize().y == hop.getHeight());

    // Controllo pixel secondo la soglia
    CHECK(drawable.image.getPixel(0, 0) == sf::Color::White);
    CHECK(drawable.image.getPixel(1, 0) == sf::Color::Black);
    CHECK(drawable.image.getPixel(2, 0) == sf::Color::White);
    CHECK(drawable.image.getPixel(0, 1) == sf::Color::Black);
    CHECK(drawable.image.getPixel(1, 1) == sf::Color::White);
    CHECK(drawable.image.getPixel(2, 1) == sf::Color::Black);
    CHECK(drawable.image.getPixel(0, 2) == sf::Color::Black);
    CHECK(drawable.image.getPixel(1, 2) == sf::Color::Black);
    CHECK(drawable.image.getPixel(2, 2) == sf::Color::Black);
  }

  SUBCASE("Testing a 4x4 image") {
    nn::Hopfield hop(4, 4);

    sf::Image img;
    img.create(4, 4, sf::Color::Black);

    img.setPixel(0, 0, sf::Color(131, 27, 254));
    img.setPixel(1, 2, sf::Color(34, 183, 200));
    img.setPixel(3, 3, sf::Color(227, 145, 67));
    nn::Pattern pat{hop.pattern(img)};
    nn::Drawable drawable{hop.baw_image(pat)};

    CHECK(drawable.image.getSize().x == hop.getWidth());
    CHECK(drawable.image.getSize().y == hop.getHeight());

    CHECK(drawable.image.getPixel(0, 0) == sf::Color::White);
    CHECK(drawable.image.getPixel(1, 1) == sf::Color::Black);
    CHECK(drawable.image.getPixel(1, 2) == sf::Color::White);
    CHECK(drawable.image.getPixel(2, 2) == sf::Color::Black);
    CHECK(drawable.image.getPixel(3, 3) == sf::Color::White);
  }
}

TEST_CASE("Testing that each image is converted to black and white") {
  nn::Hopfield hop(4, 5);  // o la dimensione che ti serve

  SUBCASE("Avogadro") {
    sf::Image img{hop.loadImage("Avogadro.png")};
    nn::Pattern pat{hop.pattern(img)};

    nn::Drawable drawable{hop.baw_image(pat)};

    CHECK(drawable.image.getSize().x == hop.getWidth());
    CHECK(drawable.image.getSize().y == hop.getHeight());

    for (unsigned i{0}; i < hop.getWidth(); ++i) {
      for (unsigned j{0}; j < hop.getHeight(); ++j) {
        sf::Color color{drawable.image.getPixel(i, j)};
        CHECK((color == sf::Color::Black || color == sf::Color::White));
      }
    }
  }

  SUBCASE("Einstein") {
    sf::Image img{hop.loadImage(
        "Einstein.png")};  // forse non è necessario caricare l'immagine
    nn::Pattern pat{hop.pattern(img)};

    nn::Drawable drawable{hop.baw_image(pat)};

    CHECK(drawable.image.getSize().x == hop.getWidth());
    CHECK(drawable.image.getSize().y == hop.getHeight());

    for (unsigned i{0}; i < hop.getWidth(); ++i) {
      for (unsigned j{0}; j < hop.getHeight(); ++j) {
        sf::Color color = drawable.image.getPixel(i, j);
        CHECK((color == sf::Color::Black || color == sf::Color::White));
      }
    }
  }
}

TEST_CASE("Testing corruption ") {
  nn::Hopfield hop(3, 4);

  SUBCASE("Einstein") {
    sf::Image img{hop.loadImage(
        "Einstein.png")};  // forse non è necessario caricare l'immagine
    nn::Pattern pat{hop.pattern(img)};
    nn::Drawable drawable{hop.baw_image(pat)};
    nn::Pattern corrupted{hop.corruption(pat, 2)};

    CHECK(corrupted.size() == pat.size());

    for (auto val : corrupted) {
      CHECK((val == 1 || val == -1));
    }

    bool changed{false};
    for (unsigned i{0}; i < pat.size(); ++i) {
      if (pat[i] != corrupted[i]) {
        changed = true;
        break;
      }
    }
    CHECK(changed == true);
  }

  SUBCASE("Curie") {
    sf::Image img = hop.loadImage(
        "Curie.png");  // forse non è necessario caricare l'immagine
    nn::Pattern pat{hop.pattern(img)};
    nn::Drawable drawable{hop.baw_image(pat)};
    nn::Pattern corrupted{hop.corruption(pat, 1)};

    CHECK(corrupted.size() == pat.size());

    for (auto val : corrupted) {
      CHECK((val == 1 || val == -1));
    }

    bool changed{false};
    for (unsigned i{0}; i < pat.size(); ++i) {
      if (pat[i] != corrupted[i]) {
        changed = true;
        break;
      }
    }
    CHECK(changed == true);
  }
}

TEST_CASE("Testing update function with zero-initialized weights") {
  SUBCASE(
      "Any input pattern should transform to all ones due to zero weights") {
    nn::Hopfield hop(2, 2);
    unsigned N{hop.getN()};
    nn::Matrix W(N, std::vector<double>(N, 0.));
    nn::Pattern pat = {1, -1, 1, -1};
    nn::Pattern expectedPat(N, 1);
    auto newPat{hop.update(pat, W)};
    auto last_index{newPat.size() - 1};

    std::cout << newPat.size() << '\n';

    CHECK(newPat[last_index][0] == 1);
    CHECK(newPat[last_index][1] == 1);
    CHECK(newPat[last_index][2] == 1);
    CHECK(newPat[last_index][3] == 1);

    CHECK(newPat[last_index] == expectedPat);
  }

  SUBCASE("A 4x4 pattern should transform to all ones due to zero weights") {
    nn::Hopfield hop(4, 4);
    unsigned N{hop.getN()};
    nn::Matrix W(N, std::vector<double>(N, 0.));

    nn::Pattern pattern = {1, -1, 1, -1, -1, 1, -1, 1,
                           1, -1, 1, -1, -1, 1, -1, 1};

    CHECK(pattern.size() == N);
    nn::Pattern expectedPattern(N, 1);

    auto actualPattern{hop.update(pattern, W)};
    auto last_index{actualPattern.size() - 1};

    CHECK(actualPattern[last_index] == expectedPattern);
  }
}

TEST_CASE("Testing neural network with 4 neurons") {
  nn::Hopfield hop(2, 2);

  SUBCASE("Testing neural network with 2 patterns") {
    nn::Pattern p1 = {-1, 1, 1, -1};
    nn::Pattern p2 = {1, -1, -1, 1};
    std::vector<nn::Pattern> patterns = {p1, p2};

    hop.matrix(patterns);
    // bisogna testare che scrive su file???? come?
    nn::Matrix W{hop.getMatrix()};

    nn::Matrix expected_matrix = {{0., -0.5, -0.5, 0.5},
                                  {-0.5, 0., 0.5, -0.5},
                                  {-0.5, 0.5, 0., -0.5},
                                  {0.5, -0.5, -0.5, 0.}};

    for (unsigned i{0}; i < hop.getN(); ++i) {
      for (unsigned j{0}; j < hop.getN(); ++j) {
        CHECK(W[i][j] == expected_matrix[i][j]);
      }
    }
    nn::Pattern corrupted = {1, -1, 1, -1};

    auto updated{hop.update(corrupted, W)};
    auto last_index{updated.size() - 1};
    CHECK(updated[last_index] == p1);

    for (unsigned i{1}; i < updated.size(); ++i) {
      CHECK(hop.energy(updated[i]) <= hop.energy(updated[i - 1]));
    }
  }
}
