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

    Pattern black_pattern{hop.pattern(total_black)};
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

    Pattern black_pattern{hop.pattern(total_white)};
    std::vector<int> v(hop.getN(), 1);

    CHECK(black_pattern == v);
  }
}

// TEST_CASE("Testing resizeimage with 2x2 image") {

//   Hopfield hop(2, 2);
//
//   sf::Image image;
//   image.create(3, 3, sf::Color::White);
//   image.setPixel(0, 0, sf::Color::Black);
//   image.setPixel(0, 1, sf::Color(255, 0, 0));
//   image.setPixel(0, 2, sf::Color::White);
//   image.setPixel(1, 0, sf::Color(0, 255, 0));
//   image.setPixel(1, 1, sf::Color(0, 0, 255));
//   image.setPixel(1, 2, sf::Color(0, 255, 255));
//   image.setPixel(2, 0, sf::Color(50, 85, 70));
//   image.setPixel(2, 1, sf::Color(245, 245, 220));
//   image.setPixel(2, 2, sf::Color(7, 14, 28));
//
//   auto resized = hop.resizeimage(image);
//
//   CHECK(resized.size() == 4);
//
//   CHECK(resized[0].pr == 0);
//   CHECK(resized[0].pg == 0);
//   CHECK(resized[0].pb == 0);
//
//   CHECK(resized[1].pr == 255);
//   CHECK(resized[1].pg == 0);
//   CHECK(resized[1].pb == 0);
//
//   CHECK(resized[2].pr == 0);
//   CHECK(resized[2].pg == 255);
//   CHECK(resized[2].pb == 0);
//
//   CHECK(resized[3].pr == 0);
//   CHECK(resized[3].pg == 0);
//   CHECK(resized[3].pb == 255);
// }