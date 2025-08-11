#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "hopfield.hpp"

//TEST_CASE("Testing resizeimage with 2x2 iamge") {
//  Hopfield hop;
//
//    sf::Image image;
//  image.create(2, 2);
//  image.setPixel(0, 0, sf::Color::Black);
//  image.setPixel(0, 1, sf::Color::Red);
//  image.setPixel(1, 0, sf::Color::Green);
//  image.setPixel(1, 1, sf::Color::Blue);
//
//  auto resized = hop.resizeimage(image);
//
//  CHECK(resized.size() == hop.getWidth() * hop.getHeight());
//  if (resized.size() == hop.getWidth() * hop.getHeight()) {
//    CHECK(resized[0].pr == 0);
//    CHECK(resized[0].pg == 0);
//    CHECK(resized[0].pb == 0);
//
//    CHECK(resized[1].pr == 255);
//    CHECK(resized[1].pg == 0);
//    CHECK(resized[1].pb == 0);
//
//    CHECK(resized[2].pr == 0);
//    CHECK(resized[2].pg == 255);
//    CHECK(resized[2].pb == 0);
//
//    CHECK(resized[3].pr == 0);
//    CHECK(resized[3].pg == 0);
//    CHECK(resized[3].pb == 255);
//  }
//}