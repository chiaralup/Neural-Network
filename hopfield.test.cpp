#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "hopfield.hpp"

#include "doctest.h"

TEST_CASE("Testing loadImage and loadSprite functions") {
  nn::Hopfield hop(42, 51);

  REQUIRE(hop.getWidth() == 42);
  REQUIRE(hop.getHeight() == 51);
  REQUIRE(hop.getN() == 42 * 51);

  SUBCASE("Loading a valid image") {
    nn::Drawable drawable{hop.loadSprite("Avogadro.png")};
    CHECK(drawable.image.getSize().x > 0);
    CHECK(drawable.image.getSize().y > 0);
    CHECK(drawable.texture.getSize().x > 0);
    CHECK(drawable.texture.getSize().y > 0);
    CHECK(drawable.sprite.getTexture() != nullptr);
  }

  SUBCASE("Loading an invalid image") {
    CHECK_THROWS(hop.loadImage("Pauli.png"));
    CHECK_THROWS(hop.loadSprite("Pauli.png"));
  }
}

TEST_CASE("Testing interpolation function") {
  nn::Hopfield hop(2, 2);

  SUBCASE("interpolation at the center of the image") {
    unsigned p1{0}, p2{100}, p3{200}, p4{250};
    double s{0.5};
    double t{0.5};
    sf::Uint8 result{hop.interpolation(p1, p2, p3, p4, s, t)};
    CHECK(result == 138);
  }

  SUBCASE("interpolation at the edges of the image") {
    unsigned p1{50}, p2{100}, p3{150}, p4{200};

    CHECK(hop.interpolation(p1, p2, p3, p4, 0.0, 0.0) == 50);
    CHECK(hop.interpolation(p1, p2, p3, p4, 1.0, 0.0) == 100);
    CHECK(hop.interpolation(p1, p2, p3, p4, 0.0, 1.0) == 150);
    CHECK(hop.interpolation(p1, p2, p3, p4, 1.0, 1.0) == 200);
  }

  SUBCASE("Interpolation at the extremes 0 and 255") {
    unsigned p1{0}, p2{255}, p3{0}, p4{255};
    double s{0.75};
    double t{0.25};
    sf::Uint8 result{hop.interpolation(p1, p2, p3, p4, s, t)};
    CHECK(result == 191);
  }

  SUBCASE("Asymmetric interpolation test") {
    unsigned p1{10}, p2{50}, p3{100}, p4{200};
    double s{0.25};
    double t{0.75};

    sf::Uint8 expected{99};

    sf::Uint8 result{hop.interpolation(p1, p2, p3, p4, s, t)};
    CHECK(result == expected);
  }
}

TEST_CASE("Testing resizeImage function") {
  SUBCASE("Image height is too small") {
    nn::Hopfield hop(3, 3);
    sf::Image img;
    img.create(3, 1, sf::Color::Black);

    CHECK_THROWS(hop.resizeImage(img),
                 "resize_image: too small image, both dimensions must be >= 3");
  }

  SUBCASE("Image width is too small") {
    nn::Hopfield hop(3, 3);
    sf::Image img;
    img.create(1, 3, sf::Color::Black);

    CHECK_THROWS(hop.resizeImage(img),
                 "resize_image: too small image, both dimensions must be >= 3");
  }

  SUBCASE("Testing a total black image") {
    nn::Hopfield hop(2, 2);
    sf::Image total_black;
    total_black.create(3, 3, sf::Color::Black);
    auto resized{hop.resizeImage(total_black)};

    CHECK(resized.size() == hop.getN());

    for (unsigned i{0}; i < hop.getN(); ++i) {
      CHECK(resized[i].r == 0);
      CHECK(resized[i].g == 0);
      CHECK(resized[i].b == 0);
    }

    nn::Pattern black_pattern{hop.pattern(total_black)};
    std::vector<int> v(hop.getN(), -1);

    CHECK(black_pattern == v);

    nn::Drawable black_drawable{hop.bawImage(black_pattern)};

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
    nn::Hopfield hop(2, 2);
    sf::Image total_white;
    total_white.create(3, 3, sf::Color::White);
    auto resized{hop.resizeImage(total_white)};

    CHECK(resized.size() == hop.getN());

    for (unsigned i{0}; i < hop.getN(); ++i) {
      CHECK(resized[i].r == 255);
      CHECK(resized[i].g == 255);
      CHECK(resized[i].b == 255);
    }

    nn::Pattern white_pattern{hop.pattern(total_white)};
    std::vector<int> v(hop.getN(), 1);

    CHECK(white_pattern == v);

    nn::Drawable white_drawable{hop.bawImage(white_pattern)};

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
    nn::Hopfield hop(2, 2);
    sf::Image image;
    image.create(3, 3, sf::Color::Black);
    image.setPixel(0, 0, sf::Color(200, 127, 150));
    image.setPixel(0, 1, sf::Color(28, 34, 160));
    image.setPixel(1, 0, sf::Color(63, 164, 102));
    image.setPixel(1, 1, sf::Color(150, 87, 255));
    image.setPixel(2, 0, sf::Color(221, 126, 60));
    image.setPixel(2, 2, sf::Color(255, 155, 0));

    auto resized{hop.resizeImage(image)};
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

    nn::Drawable drawable{hop.bawImage(pattern)};

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

  SUBCASE("Testing downscaling a 4x4 image to 2x2 with interpolation") {
    nn::Hopfield hop(2, 2);
    sf::Image image;
    image.create(4, 4, sf::Color::Black);

    image.setPixel(0, 0, sf::Color(11, 25, 47));
    image.setPixel(1, 0, sf::Color(200, 240, 34));
    image.setPixel(0, 1, sf::Color(79, 134, 255));
    image.setPixel(1, 1, sf::Color(40, 209, 176));
    image.setPixel(2, 2, sf::Color(154, 87, 45));
    image.setPixel(3, 2, sf::Color(54, 222, 245));
    image.setPixel(2, 3, sf::Color(12, 90, 235));
    image.setPixel(3, 3, sf::Color(154, 237, 80));

    auto resized{hop.resizeImage(image)};
    CHECK(resized.size() == hop.getN());

    CHECK(resized[0].r == 11);
    CHECK(resized[0].g == 25);
    CHECK(resized[0].b == 47);

    CHECK(resized[1].r == 0);
    CHECK(resized[1].g == 0);
    CHECK(resized[1].b == 0);

    CHECK(resized[2].r == 0);
    CHECK(resized[2].g == 0);
    CHECK(resized[2].b == 0);

    CHECK(resized[3].r == 154);
    CHECK(resized[3].g == 87);
    CHECK(resized[3].b == 45);

    nn::Pattern pattern = hop.pattern(image);

    std::vector<int> expected_pattern = {-1, -1, -1, -1};
    CHECK(pattern == expected_pattern);
  }
}

TEST_CASE("Testing loadPatterns function") {
  nn::Hopfield hop(42, 51);

  std::vector<std::string> files = {"Avogadro.png", "Curie.png",
                                    "Einstein.png", "Heisenberg.png",
                                    "Hopfield.png", "Schrodinger.png"};

  SUBCASE("Should load the correct number of patterns and correct size") {
    std::vector<nn::Pattern> loadedPatterns = hop.loadPatterns();

    CHECK(loadedPatterns.size() == files.size());

    for (const auto& pat : loadedPatterns) {
      CHECK(pat.size() == hop.getN());
    }

    if (!loadedPatterns.empty()) {
      for (const auto& pat : loadedPatterns) {
        CHECK(!pat.empty());
        for (int val : pat) {
          CHECK((val == -1 || val == 1));
        }
      }
    }
  }
}

TEST_CASE("Testing pattern function") {
  SUBCASE("Test a simple 3x3 image") {
    nn::Hopfield hop(3, 3);
    unsigned N{hop.getN()};
    sf::Image img;
    img.create(3, 3, sf::Color::Black);

    // Set a few pixels with specific colors
    img.setPixel(0, 0, sf::Color(100, 88, 34));
    img.setPixel(1, 0, sf::Color(154, 120, 230));
    img.setPixel(2, 2, sf::Color(13, 25, 67));
    img.setPixel(1, 1, sf::Color(200, 253, 127));

    auto pat{hop.pattern(img)};

    CHECK(pat.size() == N);

    CHECK(pat[0] == -1);
    CHECK(pat[1] == 1);
    CHECK(pat[2] == -1);
    CHECK(pat[3] == -1);
    CHECK(pat[4] == 1);
    CHECK(pat[5] == -1);
    CHECK(pat[6] == -1);
    CHECK(pat[7] == -1);
    CHECK(pat[8] == -1);
  }
}

TEST_CASE("Testing bawImage function") {
  SUBCASE("Testing a 3x3 image") {
    nn::Hopfield hop(3, 3);

    sf::Image img;
    img.create(3, 3, sf::Color::Black);
    img.setPixel(0, 0, sf::Color(200, 127, 150));
    img.setPixel(1, 1, sf::Color(150, 87, 255));
    img.setPixel(2, 0, sf::Color(168, 59, 207));

    auto pat{hop.pattern(img)};

    nn::Drawable drawable{hop.bawImage(pat)};

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
    nn::Drawable drawable{hop.bawImage(pat)};

    CHECK(drawable.image.getSize().x == hop.getWidth());
    CHECK(drawable.image.getSize().y == hop.getHeight());

    CHECK(drawable.image.getPixel(0, 0) == sf::Color::White);
    CHECK(drawable.image.getPixel(1, 1) == sf::Color::Black);
    CHECK(drawable.image.getPixel(1, 2) == sf::Color::White);
    CHECK(drawable.image.getPixel(2, 2) == sf::Color::Black);
    CHECK(drawable.image.getPixel(3, 3) == sf::Color::White);
  }

  SUBCASE("Testing with Avogadro.png") {
    nn::Hopfield hop(4, 5);
    sf::Image img{hop.loadImage("Avogadro.png")};
    nn::Pattern pat{hop.pattern(img)};

    nn::Drawable drawable{hop.bawImage(pat)};

    CHECK(drawable.image.getSize().x == hop.getWidth());
    CHECK(drawable.image.getSize().y == hop.getHeight());

    for (unsigned i{0}; i < hop.getWidth(); ++i) {
      for (unsigned j{0}; j < hop.getHeight(); ++j) {
        sf::Color color{drawable.image.getPixel(i, j)};
        CHECK((color == sf::Color::Black || color == sf::Color::White));
      }
    }
  }

  SUBCASE("Testing with Einstein.png") {
    nn::Hopfield hop(6, 5);
    sf::Image img{hop.loadImage("Einstein.png")};
    nn::Pattern pat{hop.pattern(img)};

    nn::Drawable drawable{hop.bawImage(pat)};

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

TEST_CASE("Testing corruption function") {
  nn::Hopfield hop(3, 4);

  SUBCASE("Einstein") {
    sf::Image img{hop.loadImage("Einstein.png")};
    nn::Pattern pat{hop.pattern(img)};
    nn::Drawable drawable{hop.bawImage(pat)};
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
    sf::Image img{hop.loadImage("Curie.png")};
    nn::Pattern pat{hop.pattern(img)};
    nn::Drawable drawable{hop.bawImage(pat)};
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

TEST_CASE("Testing corruption function with ratio") {
  nn::Hopfield hop(3, 4);
  unsigned N{hop.getN()};
  nn::Pattern pat(N, 1);

  SUBCASE("Ratio is 0, should throw an exception") {
    CHECK_THROWS(hop.corruption(pat, 0), "ratio must be > 0");
  }

  SUBCASE("Ratio is a divisor of the pattern size") {
    nn::Pattern corrupted{hop.corruption(pat, 4)};

    CHECK(corrupted.size() == pat.size());

    for (auto val : corrupted) {
      CHECK((val == 1 || val == -1));
    }

    int flipped_count{0};
    for (unsigned i{0}; i < corrupted.size(); ++i) {
      if (pat[i] != corrupted[i]) {
        ++flipped_count;
      }
    }
    CHECK(flipped_count == 3);
  }

  SUBCASE("Ratio is not a divisor of the pattern size") {
    nn::Pattern corrupted{hop.corruption(pat, 5)};

    CHECK(corrupted.size() == pat.size());

    for (auto val : corrupted) {
      CHECK((val == 1 || val == -1));
    }

    int flipped_count{0};
    for (unsigned i{0}; i < corrupted.size(); ++i) {
      if (pat[i] != corrupted[i]) {
        ++flipped_count;
      }
    }

    CHECK(flipped_count == 2);
  }

  SUBCASE("Ratio is greater than the pattern size") {
    nn::Pattern corrupted = hop.corruption(pat, 20);

    CHECK(corrupted.size() == pat.size());

    for (auto val : corrupted) {
      CHECK((val == 1 || val == -1));
    }

    CHECK(corrupted == pat);
  }
}

TEST_CASE("Testing matrix and update functions") {
  SUBCASE(
      "Any input pattern should transform to all ones due to zero weights") {
    nn::Hopfield hop(2, 2);
    unsigned N{hop.getN()};
    nn::Pattern pat = {1, -1, 1, -1};
    nn::Pattern expectedPat(N, 1);
    auto newPat{hop.update(pat)};

    std::cout << newPat.size() << '\n';

    CHECK(newPat == expectedPat);
  }

  SUBCASE("A 4x4 pattern should transform to all ones due to zero weights") {
    nn::Hopfield hop(4, 4);
    unsigned N{hop.getN()};

    nn::Pattern pattern = {1, -1, 1, -1, -1, 1, -1, 1,
                           1, -1, 1, -1, -1, 1, -1, 1};

    CHECK(pattern.size() == N);

    nn::Pattern expectedPattern(N, 1);
    auto actualPattern{hop.update(pattern)};

    CHECK(actualPattern == expectedPattern);
  }
}

TEST_CASE("Testing getMatrix and energy functions") {
  nn::Hopfield hop(2, 2);

  SUBCASE("Testing neural network with 2 patterns") {
    nn::Pattern p1 = {-1, 1, 1, -1};
    nn::Pattern p2 = {1, -1, -1, 1};
    std::vector<nn::Pattern> patterns = {p1, p2};

    hop.matrix(patterns);
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
    double before{hop.energy(corrupted)};
    auto updated{hop.update(corrupted)};
    double after{hop.energy(updated)};
    CHECK(updated == p1);

    for (unsigned i{1}; i < updated.size(); ++i) {
      CHECK(after <= before);
    }
  }
}

TEST_CASE("Testing getMatrix function") {
  SUBCASE(
      "getMatrix should throw an exception if a diagonal element is not zero") {
    std::ofstream file("weights.txt");
    file << "0.0 0.5 0.5 0.5\n";
    file << "0.5 1.0 0.5 0.5\n";
    file << "0.5 0.5 0.0 0.5\n";
    file << "0.5 0.5 0.5 0.0\n";
    file.close();
    nn::Hopfield hop(2, 2);
    CHECK_THROWS_AS(hop.getMatrix(), std::runtime_error);
  }

  SUBCASE("getMatrix should throw an exception if the file is missing") {
    std::remove("weights.txt");

    nn::Hopfield hop(2, 2);
    CHECK_THROWS_AS(hop.getMatrix(), std::runtime_error);
  }
}

TEST_CASE("Testing update and energy functions") {
  SUBCASE("An already stored pattern should not change after an update") {
    nn::Hopfield hop(2, 2);
    nn::Pattern stored_pattern = {1, -1, -1, 1};
    std::vector<nn::Pattern> patterns{stored_pattern};
    hop.matrix(patterns);

    auto updated_pattern = hop.update(stored_pattern);
    CHECK(updated_pattern == stored_pattern);
  }

  SUBCASE("A corrupted pattern should change to one of the stored patterns") {
    nn::Hopfield hop(2, 2);
    nn::Pattern p1 = {1, 1, -1, -1};
    nn::Pattern p2 = {-1, -1, 1, 1};
    std::vector<nn::Pattern> patterns = {p1, p2};
    hop.matrix(patterns);

    nn::Pattern corrupted = {1, -1, -1, -1};
    auto updated_pattern{hop.update(corrupted)};

    CHECK(updated_pattern == p1);
  }

  SUBCASE("Energy should decrease or remain the same after an update") {
    nn::Hopfield hop(2, 2);
    nn::Pattern p1 = {-1, 1, -1, 1};
    nn::Pattern p2 = {1, -1, 1, -1};
    std::vector<nn::Pattern> patterns = {p1, p2};
    hop.matrix(patterns);

    nn::Pattern corrupted = {1, 1, 1, -1};
    double before{hop.energy(corrupted)};
    auto updated{hop.update(corrupted)};
    double after{hop.energy(updated)};

    CHECK(after <= before);
  }
}