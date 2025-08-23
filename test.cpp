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
    CHECK_THROWS(hop.loadImage("avogadro.png"));
    CHECK_THROWS(hop.loadSprite("avogadro.png"));
  }
}

TEST_CASE("Testing Hopfield Neural Network") {
  nn::Hopfield hop(2, 2);
  SUBCASE("Total black image") {
    sf::Image total_black;
    total_black.create(3, 3, sf::Color::Black);
    std::vector<nn::Pixel> resized{hop.resize_image(total_black)};

    CHECK(resized.size() == hop.getN());

    for (unsigned int i{0}; i < 4; ++i) {
      CHECK(resized[i].pr == 0);
      CHECK(resized[i].pg == 0);
      CHECK(resized[i].pb == 0);
    }

    nn::Pattern black_pattern{hop.pattern(total_black)};
    std::vector<int> v(hop.getN(), -1);

    CHECK(black_pattern == v);
  }

  SUBCASE("Total white image") {
    sf::Image total_white;
    total_white.create(3, 3, sf::Color::White);
    std::vector<nn::Pixel> resized{hop.resize_image(total_white)};

    CHECK(resized.size() == hop.getN());

    for (unsigned int i{0}; i < 4; ++i) {
      CHECK(resized[i].pr == 255);
      CHECK(resized[i].pg == 255);
      CHECK(resized[i].pb == 255);
    }

    nn::Pattern black_pattern{hop.pattern(total_white)};
    std::vector<int> v(hop.getN(), 1);

    CHECK(black_pattern == v);
  }
}

TEST_CASE("Testing loadPatterns") {
  nn::Hopfield hop(42, 51);

  std::vector<std::string> files = {
      "Avogadro.png", "Curie.png",      "Einstein.png", "Feynman.png",
      "Galileo.png",  "Heisenberg.png", "Hopfield.png", "Schrodinger.png"};

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

// TEST_CASE("Testing the function baw_image") {
//  nn::Hopfield hop(2, 2);
//
//  sf::Image img;
//  img.create(2, 2, sf::Color::Black);
//  img.setPixel(0, 0, sf::Color(200, 127, 150));
//  img.setPixel(1, 1, sf::Color(150, 87, 255));
//
//  Pattern pat = hop.pattern(img);
//
//  nn::Drawable drawable = hop.baw_image(pat);
//
//  CHECK(drawable.image.getSize().x == hop.getWidth());
//  CHECK(drawable.image.getSize().y == hop.getHeight());
//
//  // Controllo pixel secondo la soglia
//  CHECK(drawable.image.getPixel(0, 0) == sf::Color::White);
//  CHECK(drawable.image.getPixel(1, 0) == sf::Color::Black);
//  CHECK(drawable.image.getPixel(0, 1) == sf::Color::Black);
//  CHECK(drawable.image.getPixel(1, 1) == sf::Color::White);
//}
//
// TEST_CASE("Testing baw_image") {
//  nn::Hopfield hop(4, 4);
//
//  sf::Image img;
//  img.create(4, 4, sf::Color::Black);
//
//  img.setPixel(0, 0, sf::Color(131, 27, 254));
//  img.setPixel(1, 2, sf::Color(34, 183, 200));
//  img.setPixel(3, 3, sf::Color(227, 145, 67));
//  Pattern pat = hop.pattern(img);
//  nn::Drawable drawable = hop.baw_image(pat);
//
//  CHECK(drawable.image.getSize().x == hop.getWidth());
//  CHECK(drawable.image.getSize().y == hop.getHeight());
//
//  CHECK(drawable.image.getPixel(0, 0) == sf::Color::White);
//  CHECK(drawable.image.getPixel(1, 1) == sf::Color::Black);
//  CHECK(drawable.image.getPixel(1, 2) == sf::Color::White);
//  CHECK(drawable.image.getPixel(2, 2) == sf::Color::Black);
//  CHECK(drawable.image.getPixel(3, 3) == sf::Color::White);
//}

TEST_CASE("Testing that each image is converted to black and white") {
  nn::Hopfield hop(42, 51);  // o la dimensione che ti serve

  SUBCASE("Avogadro") {
    sf::Image img{hop.loadImage("Avogadro.png")};
    nn::Pattern pat = hop.pattern(img);

    nn::Drawable drawable = hop.baw_image(pat);

    CHECK(drawable.image.getSize().x == hop.getWidth());
    CHECK(drawable.image.getSize().y == hop.getHeight());

    for (unsigned int i{0}; i < drawable.image.getSize().x; ++i) {
      for (unsigned int j{0}; j < drawable.image.getSize().y; ++j) {
        sf::Color c = drawable.image.getPixel(i, j);
        CHECK((c == sf::Color::Black || c == sf::Color::White));
      }
    }
  }

  SUBCASE("Einstein") {
    sf::Image img{hop.loadImage("Einstein.png")};
    nn::Pattern pat = hop.pattern(img);

    nn::Drawable drawable = hop.baw_image(pat);

    CHECK(drawable.image.getSize().x == hop.getWidth());
    CHECK(drawable.image.getSize().y == hop.getHeight());

    for (unsigned int i{0}; i < drawable.image.getSize().x; ++i) {
      for (unsigned int j{0}; j < drawable.image.getSize().y; ++j) {
        sf::Color c = drawable.image.getPixel(i, j);
        CHECK((c == sf::Color::Black || c == sf::Color::White));
      }
    }
  }
}

TEST_CASE("Testing corruption ") {
  nn::Hopfield hop(42, 51);

  SUBCASE("Einstein") {
    sf::Image img = hop.loadImage("Einstein.png");
    nn::Pattern pat = hop.pattern(img);
    nn::Drawable drawable = hop.baw_image(pat);
    nn::Pattern corrupted{pat};

    std::default_random_engine eng;
    std::uniform_int_distribution<unsigned int> random_pix(
        0, static_cast<unsigned int>(pat.size()) - 1);

    for (unsigned int i = 0; i < (pat.size() / 10); ++i) {
      auto a = random_pix(eng);
      auto b = random_pix(eng);
      std::swap(corrupted[a], corrupted[b]);
    }

    CHECK(corrupted.size() == pat.size());

    for (auto val : corrupted) {
      CHECK((val == 1 || val == -1));
    }

    bool changed = false;
    for (unsigned int i = 0; i < pat.size(); ++i) {
      if (pat[i] != corrupted[i]) {
        changed = true;
        break;
      }
    }
    CHECK(changed);
  }

  SUBCASE("Curie") {
    sf::Image img = hop.loadImage("Curie.png");
    nn::Pattern pat = hop.pattern(img);
    nn::Drawable drawable = hop.baw_image(pat);
    nn::Pattern corrupted{pat};

    std::default_random_engine eng;
    std::uniform_int_distribution<unsigned int> random_pix(
        0, static_cast<unsigned int>(pat.size()) - 1);

    for (unsigned int i = 0; i < (pat.size() / 10); ++i) {
      auto a = random_pix(eng);
      auto b = random_pix(eng);
      std::swap(corrupted[a], corrupted[b]);
    }

    CHECK(corrupted.size() == pat.size());

    for (auto val : corrupted) {
      CHECK((val == 1 || val == -1));
    }

    bool changed = false;
    for (unsigned int i = 0; i < pat.size(); ++i) {
      if (pat[i] != corrupted[i]) {
        changed = true;
        break;
      }
    }
    CHECK(changed);
  }
}

TEST_CASE("Testing update function with zero-initialized weights") {
  nn::Hopfield hop(2, 2);

  SUBCASE("Any input pattern should transform to all 1s due to zero weights") {
    unsigned int N = hop.getN();

    nn::Pattern pat = {1, -1, 1, -1};
    nn::Pattern expectedPat(N, 1);
    nn::Pattern newPat{hop.update(pat)};

    CHECK(newPat == expectedPat);
  }

  SUBCASE("A 4x4 pattern should transform to all ones due to zero weights") {
    nn::Hopfield hopf(4, 4);
    unsigned int N = hopf.getN();

    nn::Pattern pattern = {1, -1, 1, -1, -1, 1, -1, 1,
                           1, -1, 1, -1, -1, 1, -1, 1};

    CHECK(pattern.size() == N);
    nn::Pattern expectedPattern(N, 1);

    nn::Pattern actualPattern = hopf.update(pattern);

    CHECK(actualPattern == expectedPattern);
  }
}