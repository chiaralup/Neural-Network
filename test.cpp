#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "hopfield.hpp"

TEST_CASE("Testing resizeimage with 2x2 image") {
  Hopfield hop(2, 2);

  sf::Image image;
  image.create(3, 3, sf::Color::White);
  image.setPixel(0, 0, sf::Color::Black);
  image.setPixel(0, 1, sf::Color(255, 0, 0));
  image.setPixel(0, 2, sf::Color::White);
  image.setPixel(1, 0, sf::Color(0, 255, 0));
  image.setPixel(1, 1, sf::Color(0, 0, 255));
  image.setPixel(1, 2, sf::Color(0, 255, 255));
  image.setPixel(2, 0, sf::Color(50, 85, 70));
  image.setPixel(2, 1, sf::Color(245, 245, 220));
  image.setPixel(2, 2, sf::Color(7, 14, 28));

  auto resized = hop.resizeimage(image);

  CHECK(resized.size() == 4);

  CHECK(resized[0].pr == 0);
  CHECK(resized[0].pg == 0);
  CHECK(resized[0].pb == 0);

  CHECK(resized[1].pr == 255);
  CHECK(resized[1].pg == 0);
  CHECK(resized[1].pb == 0);

  CHECK(resized[2].pr == 0);
  CHECK(resized[2].pg == 255);
  CHECK(resized[2].pb == 0);

  CHECK(resized[3].pr == 0);
  CHECK(resized[3].pg == 0);
  CHECK(resized[3].pb == 255);
}