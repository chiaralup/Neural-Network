#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "hopfield.hpp"

//TEST_CASE("Testing loadImage and loadSprite functions") {
//  nn::Hopfield hop(42, 51);
//
//  REQUIRE(hop.getWidth() == 42);   //??? o check ???
//  REQUIRE(hop.getHeight() == 51);  //??? o check ???
//  REQUIRE(hop.getN() == 42 * 51);  //??? o check ???
//
//  SUBCASE("Loading a valid image") {
//    nn::Drawable drawable{hop.loadSprite("Avogadro.png")};
//    CHECK(drawable.image.getSize().x > 0);
//    CHECK(drawable.image.getSize().y > 0);
//    CHECK(drawable.texture.getSize().x > 0);
//    CHECK(drawable.texture.getSize().y > 0);
//    CHECK(drawable.sprite.getTexture() != nullptr);
//  }
//
//  SUBCASE("Loading an invalid image") {
//    CHECK_THROWS(hop.loadImage("Avogadro"));
//    CHECK_THROWS(hop.loadSprite("Avogadro"));
//  }
//}
//
//TEST_CASE("Testing Hopfield Neural Network") {
//  nn::Hopfield hop(2, 2);
//  SUBCASE("Testing a total black image") {
//    sf::Image total_black;
//    total_black.create(3, 3, sf::Color::Black);
//    std::vector<nn::Pixel> resized{hop.resize_image(total_black)};
//
//    CHECK(resized.size() == hop.getN());
//
//    for (unsigned int i{0}; i < 4; ++i) {
//      CHECK(resized[i].pr == 0);
//      CHECK(resized[i].pg == 0);
//      CHECK(resized[i].pb == 0);
//    }
//
//    nn::Pattern black_pattern{hop.pattern(total_black)};
//    std::vector<int> v(hop.getN(), -1);
//
//    CHECK(black_pattern == v);
//
//    nn::Drawable black_drawable{hop.baw_image(black_pattern)};
//
//    CHECK(black_drawable.image.getSize().x == hop.getWidth());
//    CHECK(black_drawable.image.getSize().y == hop.getHeight());
//
//    for (unsigned int i{0}; i < hop.getWidth(); ++i) {
//      for (unsigned int j{0}; j < hop.getHeight(); ++j) {
//        sf::Color color = black_drawable.image.getPixel(i, j);
//        CHECK(color == sf::Color::Black);
//      }
//    }
//
//    CHECK(black_drawable.texture.getSize().x == hop.getWidth());
//    CHECK(black_drawable.texture.getSize().y == hop.getHeight());
//    CHECK(black_drawable.sprite.getTexture() != nullptr);
//  }
//
//  SUBCASE("Testing a total white image") {
//    sf::Image total_white;
//    total_white.create(3, 3, sf::Color::White);
//    std::vector<nn::Pixel> resized{hop.resize_image(total_white)};
//
//    CHECK(resized.size() == hop.getN());
//
//    for (unsigned int i{0}; i < 4; ++i) {
//      CHECK(resized[i].pr == 255);
//      CHECK(resized[i].pg == 255);
//      CHECK(resized[i].pb == 255);
//    }
//
//    nn::Pattern white_pattern{hop.pattern(total_white)};
//    std::vector<int> v(hop.getN(), 1);
//
//    CHECK(white_pattern == v);
//
//    nn::Drawable white_drawable{hop.baw_image(white_pattern)};
//
//    CHECK(white_drawable.image.getSize().x == hop.getWidth());
//    CHECK(white_drawable.image.getSize().y == hop.getHeight());
//
//    for (unsigned int i{0}; i < hop.getWidth(); ++i) {
//      for (unsigned int j{0}; j < hop.getHeight(); ++j) {
//        sf::Color color{white_drawable.image.getPixel(i, j)};
//        CHECK(color == sf::Color::White);
//      }
//    }
//
//    CHECK(white_drawable.texture.getSize().x == hop.getWidth());
//    CHECK(white_drawable.texture.getSize().y == hop.getHeight());
//    CHECK(white_drawable.sprite.getTexture() != nullptr);
//  }
//
//  SUBCASE("Testing a 3x3 image") {
//    sf::Image image;
//    image.create(3, 3, sf::Color::Black);
//    image.setPixel(0, 0, sf::Color(200, 127, 150));
//    image.setPixel(0, 1, sf::Color(28, 34, 160));
//    image.setPixel(1, 0, sf::Color(63, 164, 102));
//    image.setPixel(1, 1, sf::Color(150, 87, 255));
//    image.setPixel(2, 0, sf::Color(221, 126, 60));
//    image.setPixel(2, 2, sf::Color(255, 155, 0));
//
//    std::vector<nn::Pixel> resized{hop.resize_image(image)};
//    CHECK(resized.size() == hop.getN());
//
//    CHECK(resized[0].pr == 200);
//    CHECK(resized[0].pg == 127);
//    CHECK(resized[0].pb == 150);
//    CHECK(resized[1].pr == 14);
//    CHECK(resized[1].pg == 17);
//    CHECK(resized[1].pb == 80);
//    CHECK(resized[2].pr == 142);
//    CHECK(resized[2].pg == 145);
//    CHECK(resized[2].pb == 81);
//    CHECK(resized[3].pr == 101);
//    CHECK(resized[3].pg == 61);
//    CHECK(resized[3].pb == 188);
//
//    nn::Pattern pattern{hop.pattern(image)};
//    std::vector<int> v = {1, -1, -1, 1};
//
//    CHECK(pattern == v);
//
//    nn::Drawable drawable{hop.baw_image(pattern)};
//
//    CHECK(drawable.image.getSize().x == hop.getWidth());
//    CHECK(drawable.image.getSize().y == hop.getHeight());
//
//    CHECK(drawable.image.getPixel(0, 0) == sf::Color::White);
//    CHECK(drawable.image.getPixel(1, 0) == sf::Color::Black);
//    CHECK(drawable.image.getPixel(0, 1) == sf::Color::Black);
//    CHECK(drawable.image.getPixel(1, 1) == sf::Color::White);
//
//    CHECK(drawable.texture.getSize().x == hop.getWidth());
//    CHECK(drawable.texture.getSize().y == hop.getHeight());
//    CHECK(drawable.sprite.getTexture() != nullptr);
//  }
//}
//
//TEST_CASE("Testing loadPatterns") {
//  nn::Hopfield hop(42, 51);
//
//  std::vector<std::string> files = {"Avogadro.png", "Curie.png",
//                                    "Einstein.png", "Heisenberg.png",
//                                    "Hopfield.png", "Schrodinger.png"};
//
//  SUBCASE("Should load the correct number of patterns and correct size") {
//    std::vector<nn::Pattern> loadedPatterns = hop.loadPatterns();
//
//    CHECK(loadedPatterns.size() ==
//          files.size());  // pattern caricati = numero file
//
//    for (const auto& pat : loadedPatterns) {
//      CHECK(pat.size() == hop.getN());  // dimensione giusta
//    }
//
//    if (!loadedPatterns.empty()) {  // controllo siano -1 o 1
//      for (const auto& pat : loadedPatterns) {
//        CHECK(!pat.empty());
//        for (int val : pat) {
//          CHECK((val == -1 || val == 1));
//        }
//      }
//    }
//  }
//}
//
//// NON SO SE SIANO NECESSARIE, DA VEDERE SE TOGLIERLE O LASCIARLE
//
//TEST_CASE("Testing the function baw_image") {
//  SUBCASE("Testing a 2x2 image") {
//    nn::Hopfield hop(2, 2);
//
//    sf::Image img;
//    img.create(2, 2, sf::Color::Black);
//    img.setPixel(0, 0, sf::Color(200, 127, 150));
//    img.setPixel(1, 1, sf::Color(150, 87, 255));
//
//    nn::Pattern pat = hop.pattern(img);
//
//    nn::Drawable drawable = hop.baw_image(pat);
//
//    CHECK(drawable.image.getSize().x == hop.getWidth());
//    CHECK(drawable.image.getSize().y == hop.getHeight());
//
//    // Controllo pixel secondo la soglia
//    CHECK(drawable.image.getPixel(0, 0) == sf::Color::White);
//    CHECK(drawable.image.getPixel(1, 0) == sf::Color::Black);
//    CHECK(drawable.image.getPixel(0, 1) == sf::Color::Black);
//    CHECK(drawable.image.getPixel(1, 1) == sf::Color::White);
//  }
//
//  SUBCASE("Testing a 4x4 image") {
//    nn::Hopfield hop(4, 4);
//
//    sf::Image img;
//    img.create(4, 4, sf::Color::Black);
//
//    img.setPixel(0, 0, sf::Color(131, 27, 254));
//    img.setPixel(1, 2, sf::Color(34, 183, 200));
//    img.setPixel(3, 3, sf::Color(227, 145, 67));
//    nn::Pattern pat = hop.pattern(img);
//    nn::Drawable drawable = hop.baw_image(pat);
//
//    CHECK(drawable.image.getSize().x == hop.getWidth());
//    CHECK(drawable.image.getSize().y == hop.getHeight());
//
//    CHECK(drawable.image.getPixel(0, 0) == sf::Color::White);
//    CHECK(drawable.image.getPixel(1, 1) == sf::Color::Black);
//    CHECK(drawable.image.getPixel(1, 2) == sf::Color::White);
//    CHECK(drawable.image.getPixel(2, 2) == sf::Color::Black);
//    CHECK(drawable.image.getPixel(3, 3) == sf::Color::White);
//  }
//}
//
//TEST_CASE("Testing that each image is converted to black and white") {
//  nn::Hopfield hop(4, 5);  // o la dimensione che ti serve
//
//  SUBCASE("Avogadro") {
//    sf::Image img{hop.loadImage("Avogadro.png")};
//    nn::Pattern pat = hop.pattern(img);
//
//    nn::Drawable drawable{hop.baw_image(pat)};
//
//    CHECK(drawable.image.getSize().x == hop.getWidth());
//    CHECK(drawable.image.getSize().y == hop.getHeight());
//
//    for (unsigned int i{0}; i < hop.getWidth(); ++i) {
//      for (unsigned int j{0}; j < hop.getHeight(); ++j) {
//        sf::Color color = drawable.image.getPixel(i, j);
//        CHECK((color == sf::Color::Black || color == sf::Color::White));
//      }
//    }
//  }
//
//  SUBCASE("Einstein") {
//    sf::Image img{hop.loadImage(
//        "Einstein.png")};  // forse non è necessario caricare l'immagine
//    nn::Pattern pat = hop.pattern(img);
//
//    nn::Drawable drawable = hop.baw_image(pat);
//
//    CHECK(drawable.image.getSize().x == hop.getWidth());
//    CHECK(drawable.image.getSize().y == hop.getHeight());
//
//    for (unsigned int i{0}; i < hop.getWidth(); ++i) {
//      for (unsigned int j{0}; j < hop.getHeight(); ++j) {
//        sf::Color color = drawable.image.getPixel(i, j);
//        CHECK((color == sf::Color::Black || color == sf::Color::White));
//      }
//    }
//  }
//}
//
//TEST_CASE("Testing corruption ") {
//  nn::Hopfield hop(3, 4);
//
//  SUBCASE("Einstein") {
//    sf::Image img{hop.loadImage(
//        "Einstein.png")};  // forse non è necessario caricare l'immagine
//    nn::Pattern pat{hop.pattern(img)};
//    nn::Drawable drawable{hop.baw_image(pat)};
//    nn::Pattern corrupted{hop.corruption(pat, 2)};
//
//    CHECK(corrupted.size() == pat.size());
//
//    for (auto val : corrupted) {
//      CHECK((val == 1 || val == -1));
//    }
//
//    bool changed = false;
//    for (unsigned int i = 0; i < pat.size(); ++i) {
//      if (pat[i] != corrupted[i]) {
//        changed = true;
//        break;
//      }
//    }
//    CHECK(changed == true);
//  }
//
//  SUBCASE("Curie") {
//    sf::Image img = hop.loadImage(
//        "Curie.png");  // forse non è necessario caricare l'immagine
//    nn::Pattern pat = hop.pattern(img);
//    nn::Drawable drawable = hop.baw_image(pat);
//    nn::Pattern corrupted{hop.corruption(pat, 1)};
//
//    CHECK(corrupted.size() == pat.size());
//
//    for (auto val : corrupted) {
//      CHECK((val == 1 || val == -1));
//    }
//
//    bool changed = false;
//    for (unsigned int i = 0; i < pat.size(); ++i) {
//      if (pat[i] != corrupted[i]) {
//        changed = true;
//        break;
//      }
//    }
//    CHECK(changed == true);
//  }
//}
//
//TEST_CASE("Testing update function with zero-initialized weights") {
//  SUBCASE("Any input pattern should transform to all 1s due to zero weights") {
//    nn::Hopfield hop(2, 2);
//    unsigned int N = hop.getN();
//    nn::Matrix W(N, std::vector<double>(N, 0.));
//    nn::Pattern pat = {1, -1, 1, -1};
//    nn::Pattern expectedPat(N, 1);
//    nn::Pattern newPat{hop.update(pat, W)};
//
//    CHECK(newPat == expectedPat);
//  }
//
//  SUBCASE("A 4x4 pattern should transform to all ones due to zero weights") {
//    nn::Hopfield hop(4, 4);
//    unsigned int N = hop.getN();
//    nn::Matrix W(N, std::vector<double>(N, 0.));
//
//    nn::Pattern pattern = {1, -1, 1, -1, -1, 1, -1, 1,
//                           1, -1, 1, -1, -1, 1, -1, 1};
//
//    CHECK(pattern.size() == N);
//    nn::Pattern expectedPattern(N, 1);
//
//    nn::Pattern actualPattern = hop.update(pattern, W);
//
//    CHECK(actualPattern == expectedPattern);
//  }
//}
//
//TEST_CASE("Testing neural network with 4 neurons") {
//  nn::Hopfield hop(2, 2);
//
//  SUBCASE("Testing neural network with 2 patterns") {
//    nn::Pattern p1 = {-1, 1, 1, -1};
//    nn::Pattern p2 = {1, -1, -1, 1};
//    std::vector<nn::Pattern> patterns = {p1, p2};
//
//    hop.matrix(patterns);
//    // bisogna testare che scrive su file???? come?
//    nn::Matrix W{hop.getMatrix()};
//
//    nn::Matrix expected_matrix = {{0., -0.5, -0.5, 0.5},
//                                  {-0.5, 0., 0.5, -0.5},
//                                  {-0.5, 0.5, 0., -0.5},
//                                  {0.5, -0.5, -0.5, 0.}};
//
//    for (unsigned int i = 0; i < hop.getN(); ++i) {
//      for (unsigned int j = 0; j < hop.getN(); ++j) {
//        CHECK(W[i][j] == expected_matrix[i][j]);
//      }
//    }
//
//    nn::Pattern corrupted = {1, -1, 1, -1};
//
//    nn::Pattern updated1{hop.update(corrupted, W)};
//    CHECK(updated1 == p1);
//
//    CHECK(hop.energy(updated1) <= hop.energy(corrupted));
//  }
//}
//